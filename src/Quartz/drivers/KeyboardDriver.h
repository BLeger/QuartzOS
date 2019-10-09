#ifndef QZ_KEYBOARD_DRIVER_H
#define QZ_KEYBOARD_DRIVER_H

#include <qlib/io.h>

#include "Quartz/interrupts/interrupt_list.h"
#include "Quartz/interrupts/InterruptHandler.h"
#include "Quartz/Port.h"
#include "Driver.h"
#include "keycodes.h"

class KeyboardDriver : public InterruptHandler, public Driver
{
public:
	KeyboardDriver();
	~KeyboardDriver();

	virtual uint32_t handleInterrupt(uint32_t stack_ptr);

	char keyToChar(uint8_t keycode);

	virtual void activate() override {} ;
	virtual int reset() override {
		return 0;
	};
	virtual void deactivate() override {};
private:
	Port8BitSlow command_port;
	Port8BitSlow data_port;

	bool shift = false;
	bool capslock = false;

	inline bool uppercaseEnabled() { return shift || capslock; };

	static constexpr uint8_t pic_command_port = 0x64;
	static constexpr uint8_t pic_data_port = 0x60;
};

#endif // !QZ_KEYBOARD_DRIVER_H
