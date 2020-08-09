#include "semaphor.h"
#include "KernelS.h"
#include <iostream.h>
#include <fstream.h>
void dispatch();
extern volatile unsigned int lockFlag;
extern volatile int context_switch_on_demand;
extern volatile int dispatch_lockFlag;
#define lock {\
		asm pushf;\
		asm cli;\
}


#define unlock asm popf

Semaphore::Semaphore(int a) {
	this->myImpl = new KernelSem(a);
}
int Semaphore::signal(int s)
{

	if (s < 0)
		return s;
	int retval;
	lockFlag=1;
	if (s==0){
	this->myImpl->value++;
	}
	else{this->myImpl->value+=s;}
	if (val() <= 0) {
		retval = this->myImpl->unblock(s);
	}
   lockFlag=0;

  if (dispatch_lockFlag) {dispatch();};
return retval;
}
int Semaphore::wait(Time t) {

	int retval=1;
	lockFlag=1;
	this->myImpl->value--;
	if (val() < 0) {
		retval = this->myImpl->block(t);
	}
	lockFlag=0;
	  if (dispatch_lockFlag) {dispatch();};
	return retval;
}
int Semaphore::val() const {
	return this->myImpl->value;
}
Semaphore::~Semaphore() {
	this->myImpl->~KernelSem();
	this->myImpl = 0;
}

