
#ifndef EVENT_H_
#define EVENT_H_
#include "IVTEn.h"
#define PREPAREENTRY(num, call)\
void interrupt inter##num(...); \
IVTEn newE##num(num, inter##num); \
void interrupt inter##num(...) {\
newE##num.signal();\
if (call == 1)\
newE##num.callOld();\
};
typedef unsigned char IVTNo;
class KernelEv;
class Event {
public:
 Event (IVTNo ivtNo);
 ~Event ();
 void wait ();
protected:
 friend class KernelEv;
 void signal(); // can call KernelEv
private:
 KernelEv* myImpl;
};


#endif
