#include <qlib/io.h>
#include <qlib/types.h>

#include "GlobalDescriptorTable.h"
#include "interrupts/InterruptManager.h"
#include "drivers/KeyboardDriver.h"
#include "drivers/MouseDriver.h"

typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for (constructor* i = &start_ctors; i != &end_ctors; i++) 
        (*i)();
}

extern "C" void kmain(void* multibooot_structure, uint32_t magic_number) 
{
    qlib::clear_screen();

    qlib::printf("Welcome to Quartz OS\n");

	GlobalDescriptorTable gdt{};
	InterruptManager interrupt_manager{ &gdt };
	
	KeyboardDriver keyboard{};
	interrupt_manager.addHandler(&keyboard);

	MouseDriver mouse{};
	interrupt_manager.addHandler(&mouse);

	interrupt_manager.activate();

    while(true);
}