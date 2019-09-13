#include "types.h"
#include "GlobalDescriptorTable.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

void clear_screen() 
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    for (int x = 0; x < SCREEN_WIDTH; x++) 
    {
        for (int y = 0; y < SCREEN_HEIGHT; y++) 
        {
            VideoMemory[SCREEN_WIDTH * y + x] = (VideoMemory[SCREEN_WIDTH * y + x] & 0xFF00) | ' ';
        }
    }
}

void printf(const char* str)
{
    static uint16_t* VideoMemory = (uint16_t*) 0xb8000;

    // Position on the screen
    static uint8_t cursor_x = 0, cursor_y = 0;

    for (int i = 0; str[i] != '\0'; i++) 
    {
        if (str[i] == '\n') {
            cursor_x = 0;
            cursor_y++;
        } else {
            // Keep the high bits (color) and write character
            VideoMemory[SCREEN_WIDTH * cursor_y + cursor_x] = (VideoMemory[SCREEN_WIDTH * cursor_y + cursor_x] & 0xFF00) | str[i];
            cursor_x++;
        }

        if (cursor_x >= SCREEN_WIDTH) {
            cursor_x = 0;
            cursor_y++;
        }
        
        if (cursor_y >= SCREEN_HEIGHT) 
        {
            cursor_x = 0;
            cursor_y = 0;
            clear_screen();
        }
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
    clear_screen();

    printf("Welcome to Quartz OS\n");

    GlobalDescriptorTable gdt();

    while(true);
}