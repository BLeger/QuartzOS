#ifndef QZ_ASSERT_H
#define QZ_ASSERT_H

void printf(const char*);

#define ASSERT(x) if (!x) qlib::printf("ASSERT ERROR");

#endif // !QZ_ASSERT_H
