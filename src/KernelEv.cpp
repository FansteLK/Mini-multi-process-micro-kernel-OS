#include "KernelEv.h"
#include "IVTEn.h"
#include "PCB.h"
#include "SCHEDULE.H"
#define lock {\
 		asm pushf;\
 		asm cli;\
}
extern unsigned int lockFlag;
extern volatile int dispatch_lockFlag;
#define unlock asm popf

KernelEv** KernelEv::IVT_arr = new KernelEv*[256];

KernelEv::KernelEv(unsigned char ivtNo)
{


if(IVT_arr[ivtNo]!=0)
{
	IVT_arr[ivtNo]=this;
	this->event_PCB=(PCB*)PCB::running;

}

}
KernelEv::~KernelEv()
{
//delete IVT_arr[this->ivtno];
IVT_arr[this->ivtno]=0;
this->event_PCB=0;
}
void KernelEv::signal()
{
	lockFlag=1;
	if (this->current_PCB==0) value=1;
	else
	{
		value=0;
		this->event_PCB->blocked=0;
		Scheduler::put(this->event_PCB);
		this->current_PCB=0;
	}
	lockFlag=0;
	if(dispatch_lockFlag){dispatch();}
}
void KernelEv::wait()
{
lockFlag=1;
	if(PCB::running==this->event_PCB){
if (value==0)
{
	this->current_PCB=(PCB*) PCB::running;
	this->event_PCB->blocked=1;
	lockFlag=0;
	dispatch();
	return;
}
else
{
	value=0;
}

}
	lockFlag=0;
	if(dispatch_lockFlag){dispatch();}
}
