#ifndef _IVTEn_H
#define _IVTEn_H
#include <iostream.h>
//#include <dos.h>

typedef void interrupt (*pInterrupt)(...);
class IVTEn
{
public:


	pInterrupt oldR;
	unsigned char ivtnum;
	IVTEn();
	IVTEn(unsigned char ivtno,pInterrupt inter);
	void signal();
	void callOld();
	virtual ~IVTEn();
};

#endif
