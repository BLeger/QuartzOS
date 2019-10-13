#ifndef QZ_PROCESS_H
#define QZ_PROCESS_H

#include <qlib/types.h>

using pid_t	= size_t;

class Process
{
public:
	static Process* createKernelProcess(void (*entry_point)());

private:
	enum Ring 
	{
		Ring0,
		Ring3
	};

private:
	Process(pid_t pid, Ring ring, void (*entry_point)());

	uint8_t* stack[4096];
	CPURegisters* state;
};

#endif // !QZ_PROCESS_H
