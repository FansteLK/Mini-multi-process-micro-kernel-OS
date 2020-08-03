
#ifndef PCB_h_
#define PCB_h_
#include "Thread.h"
#include "List.h"
#include "KernelS.h"
class PCB
{

	static List * list;
	static volatile PCB* running;
	static PCB * idle;
	static PCB * main_thread;
	static List* waiting;
	static ID id;
	static int *SignalState;
public:
	 static  void init()
	      {
	    	  Thread *t=new Thread(1024,0);
	    	  PCB::running=t->myPCB;
               //t->start();
	      };
		ID This_id;
		Thread* MyThread;
		Thread* MakerThread;
		virtual ~PCB();
private:
		friend void do_signals();
		friend class Signals;
		friend class Handlers;
		friend class List;
		friend class KernelEv;
		friend void d();
		friend void c();
		friend void dispatch();
	friend class Thread;
	friend class KernelSem;
	friend  void inic();

	friend void interrupt timer();
	   List* blocked_list;
	   Handlers* handler_list;
	   Signals* signal_list;
	   KernelSem* semaphore;
	    unsigned* stack ;
		unsigned bp;
		unsigned sp;
		unsigned ss;
		unsigned blocked;
		unsigned finished;
		unsigned ready;
		unsigned timer_signal;
		unsigned int quantum;
       static void wrapper();
		PCB(Thread*,StackSize,Time,void (*body)());

};
#endif
