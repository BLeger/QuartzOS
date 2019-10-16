#include "kprintf.h"

char* itoa(int val, int base) {

	static char buf[32] = { 0 };
	int i = 30;

	if (val == 0)
	{
		buf[i] = '0';
		return &buf[i];
	}

	bool negative = false;
	if (val < 0)
	{
		negative = true;
		val *= -1;
	}

	for (; val && i; --i, val /= base)
		buf[i] = "0123456789abcdef"[val % base];

	if (negative)
	{
		buf[i] = '-';
		i--;
	}

	return &buf[i + 1];

}

void kclear_screen()
{
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	for (int x = 0; x < SCREEN_WIDTH; x++)
	{
		for (int y = 0; y < SCREEN_HEIGHT; y++)
		{
			VideoMemory[SCREEN_WIDTH * y + x] = (VideoMemory[SCREEN_WIDTH * y + x] & 0xFF00) | ' ';
		}
	}
}

void kprintf(char c)
{
	static char buffer[2] = { ' ', '\0' };
	buffer[0] = c;
	kprintf(buffer);
}

void kprintf(const char* str)
{
	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	// Position on the screen
	static uint16_t cursor_x = 0, cursor_y = 0;

	for (int i = 0; str[i] != '\0'; i++)
	{
		if (str[i] == '\n') {
			cursor_x = 0;
			cursor_y++;
		}
		else {
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
			kclear_screen();
		}
	}
}