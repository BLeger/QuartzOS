#ifndef QZ_PERIPHERAL_COMPONENT_INTERCONNECT_H
#define QZ_PERIPHERAL_COMPONENT_INTERCONNECT_H

#include <qlib/io.h>

#include "Quartz/core/Port.h"
#include "Quartz/interrupts/InterruptManager.h"
#include "Quartz/drivers/Driver.h"
#include "BaseAddressRegister.h"


struct PCIDeviceDescriptor
{
	uint32_t port_base;
	uint32_t interrupt;

	uint16_t bus;
	uint16_t device;
	uint16_t function;

	uint16_t vendor_id;
	uint16_t device_id;

	uint8_t class_id;
	uint8_t subclass_id;
	uint8_t interface_id;

	uint8_t revision;
};

class PeripheralComponentInterconnect
{
public:
	PeripheralComponentInterconnect();
	~PeripheralComponentInterconnect();

	uint32_t read(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset);
	void write(uint16_t bus, uint16_t device, uint16_t function, uint32_t offset, uint32_t data);
	
	bool deviceHasFunctions(uint16_t bus, uint16_t device);

	void assignDrivers(DriverManager& driver_manager);
	PCIDeviceDescriptor getDeviceDescriptor(uint16_t bus, uint16_t device, uint16_t function);
	Driver* getDriver(PCIDeviceDescriptor device);
	BaseAddressRegister getBaseAddressRegister(uint16_t bus, uint16_t device, uint16_t function, uint16_t BAR);

private:
	Port32Bit command_port;
	Port32Bit data_port;

};

#endif // !QZ_PERIPHERAL_COMPONENT_INTERCONNECT_H
