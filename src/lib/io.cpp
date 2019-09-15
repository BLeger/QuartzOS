#include "io.h"

namespace qlib
{
	char* itoa(int val, int base) {

		static char buf[32] = { 0 };

		int i = 30;

		for (; val && i; --i, val /= base)

			buf[i] = "0123456789abcdef"[val % base];

		return &buf[i + 1];

	}

	void clear_screen()
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

	void printf(const char* str)
	{
		static uint16_t* VideoMemory = (uint16_t*)0xb8000;

		// Position on the screen
		static uint8_t cursor_x = 0, cursor_y = 0;

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
				clear_screen();
			}
		}
	}
}