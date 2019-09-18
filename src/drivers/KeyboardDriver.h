#ifndef QZ_KEYBOARD_DRIVER_H
#define QZ_KEYBOARD_DRIVER_H

#include "../interrupts/interrupt_list.h"
#include "../interrupts/InterruptHandler.h"

#include "../Port.h"
#include "../lib/io.h"

#include "keycodes.h"

class KeyboardDriver : public InterruptHandler
{
public:
	KeyboardDriver();
	~KeyboardDriver();

	virtual uint32_t handleInterrupt(uint32_t stack_ptr);

	char keyToChar(uint8_t keycode);
private:
	Port8BitSlow command_port;
	Port8BitSlow data_port;

	bool shift = false;
	bool capslock = false;

	inline bool uppercaseEnabled() { return shift || capslock; };
};

#endif // !QZ_KEYBOARD_DRIVER_H
