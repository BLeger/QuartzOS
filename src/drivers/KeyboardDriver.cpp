#include "KeyboardDriver.h"

KeyboardDriver::KeyboardDriver()
: InterruptHandler(KEYBOARD_INTERRUPT), command_port(0x64), data_port(0x60)
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
		/*printf("Key released: ");
		printf(qlib::itoa(key, 16));
		printf("\n");*/
	}
	else
	{
		qlib::printf("Key pressed: ");
		qlib::printf(qlib::itoa(key, 16));
		qlib::printf("\n");
	}

	
	return stack_ptr;
}

char KeyboardDriver::keyToChar(uint8_t keycode)
{
	switch (keycode) {
	case KEY_A: return 'A';
	case KEY_B: return 'B';
	case KEY_C: return 'C';
	case KEY_D: return 'D';
	case KEY_E: return 'E';
	case KEY_F: return 'F';
	case KEY_G: return 'G';
	case KEY_H: return 'H';
	case KEY_I: return 'I';
	case KEY_J: return 'J';
	case KEY_K: return 'K';
	case KEY_L: return 'L';
	case KEY_M: return 'M';
	case KEY_N: return 'N';
	case KEY_O: return 'O';
	case KEY_P: return 'P';
	case KEY_Q: return 'Q';
	case KEY_R: return 'R';
	case KEY_S: return 'S';
	case KEY_T: return 'T';
	case KEY_U: return 'U';
	case KEY_V: return 'V';
	case KEY_W: return 'W';
	case KEY_X: return 'X';
	case KEY_Y: return 'Y';
	case KEY_Z: return 'Z';

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

	default: return '\0';
	}
}
