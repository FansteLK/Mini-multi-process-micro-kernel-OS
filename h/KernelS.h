#include "semaphor.h"
#include "List.h"
#ifndef _KernelS_H_
#define _KernelS_H_

class KernelSem
{
private :
	friend class Semaphore;
	volatile int value;

public:
	List* sem_blocked;
	KernelSem(int value);
			~KernelSem();
			int block(Time t);
			int unblock(int s);



};




#endif
