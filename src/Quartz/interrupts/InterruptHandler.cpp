#include "InterruptHandler.h"

uint32_t InterruptHandler::handleInterrupt(uint32_t stack_ptr)
{
	return stack_ptr;
}

InterruptHandler::InterruptHandler(uint8_t interrupt_number)
	: interrupt_number(interrupt_number)
{
}

InterruptHandler::~InterruptHandler()
{
}
