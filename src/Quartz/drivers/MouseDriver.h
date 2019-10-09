#ifndef QZ_MOUSE_DRIVER_H
#define QZ_MOUSE_DRIVER_H

#include <qlib/io.h>

#include "Quartz/interrupts/interrupt_list.h"
#include "Quartz/interrupts/InterruptHandler.h"
#include "Driver.h"
#include "Quartz/Port.h"

class MouseDriver : public InterruptHandler, public Driver
{
public:
	MouseDriver();
	~MouseDriver();

	virtual uint32_t handleInterrupt(uint32_t stack_ptr);

	enum MouseButton
	{
		MOUSE_LEFT = 0x00,
		MOUSE_RIGHT = 0x01,
		MOUSE_MIDDLE = 0x02
	};

	enum MouseButtonAction
	{
		BUTTON_RELEASED = 0x00,
		BUTTON_PRESSED = 0x01
	};

	virtual void activate() {};
	virtual int reset() {
		return 0;
	};
	virtual void deactivate() {};

private:
	Port8BitSlow command_port;
	Port8BitSlow data_port;

	int8_t mouse_state[3];
	uint8_t offset;

	uint8_t buttons_last_state;

	int8_t mouse_x = 40;
	int8_t mouse_y = 12;

	static constexpr uint8_t pic_command_port = 0x64;
	static constexpr uint8_t pic_data_port = 0x60;
};

#endif // !QZ_MOUSE_DRIVER_H
