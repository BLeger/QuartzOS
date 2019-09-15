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

void printf(const char*);	

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
		printf("Key pressed: ");
		printf(qlib::itoa(key, 16));
		printf("\n");
	}

	
	return stack_ptr;
}
