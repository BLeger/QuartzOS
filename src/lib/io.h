#ifndef QZ_IO_H
#define QZ_IO_H

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

#include "../types.h"

namespace qlib
{
	char* itoa(int val, int base = 10);
	void clear_screen();
	void printf(const char*);
}

#endif // !QZ_IO_H
