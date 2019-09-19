#include "MouseDriver.h"

MouseDriver::MouseDriver()
	: InterruptHandler(MOUSE_INTERRUPT), 
	command_port(pic_command_port), 
	data_port(0x60)
{
	// Reset the mouse
	command_port.write(0xFF);
	while (data_port.read() != 0xFA);

	offset = 0;
	buttons_last_state = 0;

	command_port.write(0xA8); // Start keyboard interrupts
	command_port.write(0x20); // Get current state
	uint8_t status = data_port.read() | 2;

	command_port.write(0x60); // change current state
	data_port.write(status);

	command_port.write(0xD4);
	data_port.write(0xF4);
	data_port.read();
}

MouseDriver::~MouseDriver()
{
}

uint32_t MouseDriver::handleInterrupt(uint32_t stack_ptr)
{

	uint8_t status = command_port.read();

	// if 6-th bit of status is one there is data
	if (!(status & 0x20))
		return stack_ptr;

	mouse_state[offset] = data_port.read();
	offset = (offset + 1) % 3;

	// All informations have been gathered
	if (offset == 0)
	{
		mouse_x += mouse_state[1];
		mouse_y -= mouse_state[2];

		if (mouse_x < 0) mouse_x = 0;
		if (mouse_x >= 80) mouse_x = 79;

		if (mouse_y < 0) mouse_y = 0;
		if (mouse_y >= 25) mouse_y = 24;

		for (uint8_t i = 0; i < 3; i++)
		{
			
			if ((mouse_state[0] & (0x01 << i)) != (buttons_last_state & (0x01 << i)))
			{
				MouseButton button = static_cast<MouseButton>(i);
				MouseButtonAction action = ((mouse_state[0] & (0x01 << i)) == 0) ?
											MouseButtonAction::BUTTON_RELEASED : MouseButtonAction::BUTTON_PRESSED;
		}

		buttons_last_state = mouse_state[0];
	}

	return stack_ptr;
}