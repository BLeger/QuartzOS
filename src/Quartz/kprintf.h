#ifndef QZ_KPRINTF_H
#define QZ_KPRINTF_H

#include "core/types.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

char* itoa(int val, int base = 10);
void kclear_screen();

void kprintf(char);
void kprintf(const char*);

#endif // !QZ_KPRINTF_H
