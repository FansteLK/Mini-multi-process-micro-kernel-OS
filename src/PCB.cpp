
#include "PCB.h"
#include "SCHEDULE.h"
#include <dos.h>
#include <fstream.h>
#include "Handlers.h"
#include "Signals.h"
 int PCB::id=0;
 List * PCB::list=0;
 volatile PCB* PCB::running=0;
PCB * PCB::idle=0;
PCB * PCB::main_thread=0;
List* PCB::waiting=0;
int * PCB::SignalState= new int[16];
extern volatile unsigned lockFlag;
PCB::PCB(Thread * thread,StackSize size,Time time,void (*body)())
{



	int stacksize=size/ sizeof(unsigned);

 stack = new unsigned[stacksize];
   stack[stacksize-1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
   stack[stacksize-2]=FP_SEG(body);
   stack[stacksize-3]=FP_OFF(body);
#endif

#ifndef BCC_BLOCK_IGNORE
		ss = FP_SEG(stack + stacksize - 12);
		sp = FP_OFF(stack + stacksize - 12);
#endif
		semaphore=0;
		bp = sp;
        quantum=time;
		blocked=0;
		finished=0;
		ready=0;
         timer_signal=1;
     	MyThread=thread;
     	MakerThread=0;
     	This_id=PCB::id++;
        this->signal_list=new Signals();
        handler_list=new Handlers();
     	blocked_list=new List();

	};
void PCB:: wrapper()
{

	PCB::running->MyThread->run();
PCB_Node * temp=PCB::running->blocked_list->head;
PCB_Node * old=0;
    while(temp!=0)
    {

    	temp->elem->blocked=0;
    	Scheduler::put(temp->elem);
    	temp=temp->next;


    }

    if(PCB::running->MakerThread!=0)
    PCB::running->MakerThread->signal(1);
if(PCB::running->MyThread!=0)
	PCB::running->MyThread->signal(2);

PCB::running->finished=1;

	dispatch();

};
PCB::~PCB()
{
	delete[] stack;
	this->signal_list->deleteList();
	this->handler_list->deleteHandlers();
	this->blocked_list->deleteList();
	MyThread=0;
};



