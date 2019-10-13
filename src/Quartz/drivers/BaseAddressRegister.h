#ifndef QZ_BASE_ADDRESS_REGISTER_H
#define QZ_BASE_ADDRESS_REGISTER_H

#include "Quartz/core/types.h"

enum BaseAddressRegisterType {
	MemoryMapping = 0,
	InputOutput = 1
};

struct BaseAddressRegister
{
	bool prefetchable = false; // For memory mapping only

	uint8_t* address;
	uint32_t size;
	BaseAddressRegisterType type;
};


#endif // !QZ_BASE_ADDRESS_REGISTER_H
