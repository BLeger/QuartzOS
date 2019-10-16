#ifndef QLIB_ASSERT_H
#define QLIB_ASSERT_H

void printf(const char*);

#define ASSERT(x) if (!x) ;//qlib::printf("ASSERT ERROR");

#endif // !QLIB_ASSERT_H
