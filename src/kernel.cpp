#include "types.h"
#include "GlobalDescriptorTable.h"

void printf(const char* str)
{
    uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    for (int i = 0; str[i] != '\0'; i++) 
    {
        // Keep the high bits (color) and write character
        VideoMemory[i] = (VideoMemory[i] & 0xFF00) | str[i];
    }
}

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
    printf("Welcome to Quartz OS");

    GlobalDescriptorTable gdt();

    while(true);
}