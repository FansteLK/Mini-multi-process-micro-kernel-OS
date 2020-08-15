
#include "IVTEn.h"
#include "KernelEv.h"
#include <dos.h>
#include <fstream.h>

//pInterrupt oldR;
#define lock {\
		asm pushf;\
		asm cli;\
}

#define unlock asm popf

IVTEn::IVTEn(unsigned char ivtNo,pInterrupt interapt)
{

#ifndef BCC_BLOCK_IGNORE
	oldR = getvect(ivtNo);
#endif


#ifndef BCC_BLOCK_IGNORE
	setvect(ivtNo, interapt);
#endif

ivtnum  = ivtNo;

}
void IVTEn::callOld()
{

oldR();

};
void IVTEn::signal()
{

	if(KernelEv::IVT_arr[ivtnum]!=0){

		KernelEv::IVT_arr[ivtnum]->signal();
	}

};
IVTEn::~IVTEn() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtnum,oldR);
#endif
	oldR();

}
