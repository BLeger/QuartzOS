#ifndef QZ_INTERRUPT_HANDLER_H
#define QZ_INTERRUPT_HANDLER_H

#include "Quartz/core/types.h"

class InterruptManager;

class InterruptHandler
{
public:
	uint8_t getInterruptNumber() { return interrupt_number; };

	virtual uint32_t handleInterrupt(uint32_t stack_ptr);
protected:
	InterruptHandler(uint8_t interrupt_number);
	~InterruptHandler();

	uint8_t interrupt_number;
};

#endif // !QZ_INTERRUPT_HANDLER_H
