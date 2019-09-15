#ifndef QZ_KEYBOARD_DRIVER_H
#define QZ_KEYBOARD_DRIVER_H

#include "../interrupts/interrupt_list.h"
#include "../interrupts/InterruptHandler.h"
#include "../Port.h"
#include "../lib/io.h"

class KeyboardDriver : public InterruptHandler
{
public:
	KeyboardDriver();
	~KeyboardDriver();

	virtual uint32_t handleInterrupt(uint32_t stack_ptr);
private:
	Port8BitSlow command_port;
	Port8BitSlow data_port;

};

#endif // !QZ_KEYBOARD_DRIVER_H
