
#ifndef _THREAD_H_
#define _THREAD_H_

#include <iostream.h>
typedef unsigned long StackSize;
const StackSize defaultStackSize = 4096;
typedef unsigned int Time; // time, x 55ms
const Time defaultTimeSlice = 2; // default = 2*55ms
typedef int ID;
class PCB; // Kernel's implementation of a user's thread
typedef void (*SignalHandler)();
typedef unsigned SignalId;
class Thread
{
public:

 void start();
 void waitToComplete();
 virtual ~Thread();
 ID getId();
 static ID getRunningId();
 static Thread * getThreadById(ID id);
 void signal(SignalId signal=0);
 void registerHandler(SignalId signal, SignalHandler handler);
 void unregisterAllHandlers(SignalId id);
 void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
 void blockSignal(SignalId signal);
 static void blockSignalGlobally(SignalId signal);
 void unblockSignal(SignalId signal);
 static void unblockSignalGlobally(SignalId signal);
protected:
 friend class PCB;
 Thread (StackSize stackSize = defaultStackSize, Time timeSlice =
defaultTimeSlice);
 virtual void run() {}
private:
 PCB* myPCB;
};
void dispatch ();
#endif /* THREAD_H_ */
