#include "KeyboardDriver.h"

KeyboardDriver::KeyboardDriver()
: InterruptHandler(KEYBOARD_INTERRUPT), command_port(pic_command_port), data_port(pic_data_port)
{
	// Remove previous unwanted data
	while (command_port.read() & 0x01)
		data_port.read();

	
	command_port.write(0xAE); // Start keyboard interrupts
	command_port.write(0x20); // Get current state
	uint8_t status = (data_port.read() | 1) & ~0x10;

	command_port.write(0x60); // change current state
	data_port.write(status);

	data_port.write(0xF4);
}

KeyboardDriver::~KeyboardDriver()
{
}

uint32_t KeyboardDriver::handleInterrupt(uint32_t stack_ptr)
{
	// Fetch the key
	uint8_t key = data_port.read();

	// If most significant bit = 1 => key released, else key pressed
	bool key_released = (key & 0b10000000) == 0b10000000;
	// remove key released bit to only keep the key value
	key &= 0b01111111;

	if (key_released)
	{
		if (key == KEY_LSHIFT || key == KEY_RSHIFT) 
		{
			shift = false;
		}
	}
	else
	{
		if (key == KEY_LSHIFT || key == KEY_RSHIFT)
		{
			shift = true;
		}

		if (key == KEY_CAPSLOCK)
		{
			capslock = !capslock;
		}

		qlib::printf(keyToChar(key));
	}

	return stack_ptr;
}

char KeyboardDriver::keyToChar(uint8_t keycode)
{
	switch (keycode) {
		case KEY_A: if (uppercaseEnabled()) return 'A'; else return 'a';
		case KEY_B: if (uppercaseEnabled()) return 'B'; else return 'b';
		case KEY_C: if (uppercaseEnabled()) return 'C'; else return 'c';
		case KEY_D: if (uppercaseEnabled()) return 'D'; else return 'd';
		case KEY_E: if (uppercaseEnabled()) return 'E'; else return 'e';
		case KEY_F: if (uppercaseEnabled()) return 'F'; else return 'f';
		case KEY_G: if (uppercaseEnabled()) return 'G'; else return 'g';
		case KEY_H: if (uppercaseEnabled()) return 'H'; else return 'h';
		case KEY_I: if (uppercaseEnabled()) return 'I'; else return 'i';
		case KEY_J: if (uppercaseEnabled()) return 'J'; else return 'j';
		case KEY_K: if (uppercaseEnabled()) return 'K'; else return 'k';
		case KEY_L: if (uppercaseEnabled()) return 'L'; else return 'l';
		case KEY_M: if (uppercaseEnabled()) return 'M'; else return 'm';
		case KEY_N: if (uppercaseEnabled()) return 'N'; else return 'n';
		case KEY_O: if (uppercaseEnabled()) return 'O'; else return 'o';
		case KEY_P: if (uppercaseEnabled()) return 'P'; else return 'p';
		case KEY_Q: if (uppercaseEnabled()) return 'Q'; else return 'q';
		case KEY_R: if (uppercaseEnabled()) return 'R'; else return 'r';
		case KEY_S: if (uppercaseEnabled()) return 'S'; else return 's';
		case KEY_T: if (uppercaseEnabled()) return 'T'; else return 't';
		case KEY_U: if (uppercaseEnabled()) return 'U'; else return 'u';
		case KEY_V: if (uppercaseEnabled()) return 'V'; else return 'v';
		case KEY_W: if (uppercaseEnabled()) return 'W'; else return 'w';
		case KEY_X: if (uppercaseEnabled()) return 'X'; else return 'x';
		case KEY_Y: if (uppercaseEnabled()) return 'Y'; else return 'y';
		case KEY_Z: if (uppercaseEnabled()) return 'Z'; else return 'z';

		case KEY_0: return '0';
		case KEY_1: return '1';
		case KEY_2: return '2';
		case KEY_3: return '3';
		case KEY_4: return '4';
		case KEY_5: return '5';
		case KEY_6: return '6';
		case KEY_7: return '7';
		case KEY_8: return '8';
		case KEY_9: return '9';

		case KEY_ENTER: return '\n';
		case KEY_SPACE: return ' ';

		default: return '\0';
	}
}
