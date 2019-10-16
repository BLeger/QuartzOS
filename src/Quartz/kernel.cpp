#include "kprintf.h"

#include "core/types.h"
#include "core/GlobalDescriptorTable.h"
#include "interrupts/InterruptManager.h"
#include "heap/MemoryManager.h"
#include "drivers/Driver.h"
#include "drivers/KeyboardDriver.h"
#include "drivers/MouseDriver.h"
#include "drivers/PeripheralComponentInterconnect.h"
#include "drivers/VGADriver.h"
#include "heap/kmalloc.h"
#include "core/Process.h"


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor* i = &start_ctors; i != &end_ctors; i++) 
        (*i)();
}

extern "C" void kmain(void* multiboot_structure, uint32_t magic_number) 
{
    kclear_screen();

    kprintf("Welcome to Quartz OS\n");

	GlobalDescriptorTable gdt{};
	InterruptManager interrupt_manager{ &gdt };
	
	DriverManager driver_manager{};

	KeyboardDriver keyboard{};
	interrupt_manager.addHandler(&keyboard);
	driver_manager.addDriver(&keyboard);

	MouseDriver mouse{};
	interrupt_manager.addHandler(&mouse);
	driver_manager.addDriver(&mouse);

	/*uint32_t* memupper = (uint32_t*)(((size_t)multiboot_structure) + 8);
	size_t heap = 10 * 1024 * 1024;
	MemoryManager memory_manager {10*1024*1024, (*memupper) * 1024 - heap - 10 * 1024 };*/

#ifdef GRAPHICS_MODE
	VGADriver VGA{};
	driver_manager.addDriver(&VGA);
	VGA.setMode(VideoMode{ 320, 200, 8 });

	int z = 0;
	for (int y = 0; y < 200; y++)
	{
		for (int x = 0; x < 320; x++)
		{
			if (z == 20)
				VGA.putPixel(x, y, 0x01);
			else
				VGA.putPixel(x, y, 0x00);

			
		}

		z++;
		if (z > 20) z = 0;
	}
#endif
	PeripheralComponentInterconnect PCI{};
	PCI.assignDrivers(driver_manager);

	interrupt_manager.activate();

	Process* p = new Process();

    while(true);
}

