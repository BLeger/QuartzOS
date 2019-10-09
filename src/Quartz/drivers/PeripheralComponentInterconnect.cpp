#include "PeripheralComponentInterconnect.h"

PeripheralComponentInterconnect::PeripheralComponentInterconnect()
	: command_port(0xCF8), 
	data_port(0xCFC)
{
}

PeripheralComponentInterconnect::~PeripheralComponentInterconnect()
{
}

uint32_t PeripheralComponentInterconnect::read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset)
{
	uint32_t pci_id = (0x1 << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11)
		| ((function & 0x07) << 8) | (offset & 0xFC);

	command_port.write(pci_id);
	uint32_t result = data_port.read();

	return result >> (8 * (offset % 4));
}

void PeripheralComponentInterconnect::write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t data)
{
	uint32_t pci_id = (0x1 << 31) | ((bus & 0xFF) << 16) | ((device & 0x1F) << 11)
		| ((function & 0x07) << 8) | (offset & 0xFC);

	command_port.write(pci_id);
	data_port.write(data);
}

bool PeripheralComponentInterconnect::deviceHasFunctions(uint16_t bus, uint16_t device)
{
	return read(bus, device, 0, 0x0E) & (1 << 7);
}

void PeripheralComponentInterconnect::assignDrivers(DriverManager& driver_manager)
{
	for (int bus = 0; bus < 8; bus++)
	{
		for (int device = 0; device < 32; device++)
		{
			int nb_functions = (deviceHasFunctions(bus, device)) ? 8 : 1;
			for (int function = 0; function < nb_functions; function++)
			{
				auto device_descriptor = getDeviceDescriptor(bus, device, function);
			
				if (device_descriptor.vendor_id == 0x0 || device_descriptor.vendor_id == 0xFFFF)
				{
					// This device has no more functions
					continue;
				}

				for (int32_t bar_index = 0; bar_index < 6; bar_index++) {
					BaseAddressRegister bar = getBaseAddressRegister(bus, device, function, bar_index);

					// For now, ignore memory mapping
					if (bar.address && bar.type == BaseAddressRegisterType::InputOutput)
					{
						device_descriptor.port_base = (uint32_t)(bar.address);
					}

					Driver* driver = getDriver(device_descriptor);
					if (driver != NULL) {
						driver_manager.addDriver(driver);
					}
				}

				qlib::printf("PCI BUS: ");
				qlib::printf(qlib::itoa(device_descriptor.bus & 0xFF));

				qlib::printf(" Device: ");
				qlib::printf(qlib::itoa(device_descriptor.device & 0xFF));

				qlib::printf(" Function: ");
				qlib::printf(qlib::itoa(device_descriptor.function & 0xFF));

				qlib::printf(" Vendor: ");
				qlib::printf(qlib::itoa((device_descriptor.vendor_id & 0xFF00) >> 8));
				qlib::printf(qlib::itoa(device_descriptor.vendor_id & 0xFF));

				qlib::printf(" Device: ");
				qlib::printf(qlib::itoa((device_descriptor.device_id & 0xFF00) >>  8));
				qlib::printf(qlib::itoa(device_descriptor.device_id & 0xFF));
				qlib::printf("\n");
			}
		}
	}
}

PCIDeviceDescriptor PeripheralComponentInterconnect::getDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function)
{
	PCIDeviceDescriptor device_descriptor;

	device_descriptor.bus = bus;
	device_descriptor.device = device;
	device_descriptor.function = function;

	device_descriptor.vendor_id = read(bus, device, function, 0x00);
	device_descriptor.device_id = read(bus, device, function, 0x02);

	device_descriptor.class_id = read(bus, device, function, 0x0b);
	device_descriptor.subclass_id = read(bus, device, function, 0x0a);
	device_descriptor.interface_id = read(bus, device, function, 0x09);

	device_descriptor.revision = read(bus, device, function, 0x08);
	device_descriptor.interrupt = read(bus, device, function, 0x3c);

	return device_descriptor;
}

Driver* PeripheralComponentInterconnect::getDriver(PCIDeviceDescriptor device)
{
	switch (device.vendor_id)
	{
	case 0x1022: // AMD
		switch (device.device_id)
		{
		case 0x2000: // am79c973
			qlib::printf("AMD am79c973 ");
			
			break;
		}
		break;

	case 0x8086: // Intel
		break;
	}


	switch (device.class_id)
	{
	case 0x03: // graphics
		switch (device.subclass_id)
		{
		case 0x00: // VGA
			qlib::printf("VGA ");
			break;
		}
		break;
	}

	return 0;
}

BaseAddressRegister PeripheralComponentInterconnect::getBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t BAR)
{
	BaseAddressRegister result{};

	uint32_t header_type = read(bus, device, function, 0x0E) & 0x7F;

	uint32_t max_BARs = 6 - (4 * header_type); // Only two bar for 64-bit
	if (BAR > max_BARs)
	{
		result.address = NULL;
		return result;
	}

	uint32_t bar_value = read(bus, device, function, 0x10 + 4 * BAR);
	result.type = (bar_value & 0x1) ? BaseAddressRegisterType::InputOutput : BaseAddressRegisterType::MemoryMapping; // CAST ?

	uint32_t temp;

	if (result.type == BaseAddressRegisterType::MemoryMapping) 
	{
		result.prefetchable = ((bar_value >> 3) & 0x1) == 0x1;
		// TODO
		switch ((bar_value >> 1) & 0x3)
		{
		case 0:
			// 32 bit
			break;
		case 1:
			// 20 bit
			break;
		case 2:
			// 64 bit
			break;
		default:
			break;
		}
	} 
	else // InputOutput
	{
		result.address = (uint8_t*)(bar_value & ~0x3);
		result.prefetchable = false;
	}

	return result;
}
