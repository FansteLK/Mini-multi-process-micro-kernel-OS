#include "event.h"
#include "KernelEv.h"
#include "IVTEn.h"
void Event::signal()
{
	this->myImpl->signal();
}
void Event::wait()
{
	this->myImpl->wait();
}
Event::Event(unsigned char ivt)
{
this->myImpl=new KernelEv(ivt);

}
Event::~Event()
{
	delete myImpl;
//	myImpl->~KernelEv();
      myImpl=0;
}
