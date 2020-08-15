#include "PCB.h"

#ifndef _KERNELEV_H_
#define _KERNELEV_H_

class KernelEv
{
public:
static KernelEv** IVT_arr;
unsigned int value;
unsigned char ivtno;
PCB* event_PCB;
PCB* current_PCB;
KernelEv(unsigned char ivtNo);
void signal();
void wait();
 ~KernelEv ();
};



#endif
