#include "PCB.h"
#include "SCHEDULE.H"
#include "List.h"
#include <fstream.h>
#include "semaphor.h"
#include "Signals.h"
#include "Handlers.h"
unsigned oldTimerOFF, oldTimerSEG;
volatile unsigned int lockFlag = 0;
volatile unsigned int signalFlag=0;
#define lock {\
 		asm pushf;\
 		asm cli;\
}

#define unlock asm popf

void interrupt timer();
int global_dummy;
PCB_Node* global_temp;
PCB_Node* global_old;
unsigned tsp;
unsigned tss;
unsigned tbp;
volatile int cntr = 20;
volatile int context_switch_on_demand = 0;
volatile int dispatch_lockFlag = 0;

// zabranjuje prekide

Thread::Thread(StackSize stackSize, Time timeSlice) {

	myPCB = new PCB(this, stackSize, timeSlice, PCB::wrapper);
	 if((PCB::running->MyThread!=0)&&(PCB::running!=0)) myPCB->MakerThread=PCB::running->MyThread;
	 if(PCB::running){
		 Handler_Node * temp =PCB::running->handler_list->head;
		 while(temp){
	myPCB->handler_list->addHandler(temp->signal_num,temp->body);
	temp=temp->next;
		 }
	 }
	PCB::list->addPCB(myPCB);

}
ID Thread::getId() {
	return this->myPCB->This_id;
}
ID Thread::getRunningId() {
	return PCB::running->This_id;
}
Thread* Thread::getThreadById(ID id) {
	if (PCB::list->getPCB(id) != 0)
		return PCB::list->getPCB(id)->MyThread;
	else
		return 0;
}
void Thread::start() {

	if (this->myPCB->ready)
		return;
	this->myPCB->ready = 1;
	Scheduler::put(myPCB);

}
void Thread::waitToComplete() {
	if (myPCB->finished)
		return;
	PCB::running->blocked = 1;
	myPCB->blocked_list->addPCB((PCB*) PCB::running);
	dispatch();
}
void dispatch() {
	lock;
	context_switch_on_demand = 1;
	timer();
unlock;
}
Thread::~Thread() {

this->waitToComplete();
PCB::list->deletePCB(this->getId());
myPCB->blocked_list->deleteList();
myPCB = 0;

}

// nova prekidna rutina tajmera
void tick();
void interrupt timer() {

if (!context_switch_on_demand) {
	tick();
	cntr--;
	PCB::waiting->decPCB();
}
if (cntr == 0 || context_switch_on_demand) {

	if (!lockFlag) {

	asm {

		mov tsp, sp
		mov tss, ss
		mov tbp,bp
	}
	dispatch_lockFlag = 0;
	PCB::running->sp = tsp;
	PCB::running->ss = tss;
	PCB::running->bp = tbp;

	if ((!PCB::running->finished) && (!PCB::running->blocked)
	/*&& (PCB::running->ready)*/) {
		if ((PCB::running->This_id != PCB::idle->This_id))
			Scheduler::put((PCB*) PCB::running);
	}
	PCB::running = Scheduler::get();
	if (PCB::running != 0)   do_signals();
	if(PCB::running != 0)
	while (PCB::running->finished==1) {
		PCB::running = Scheduler::get();

	}

	if ((PCB::running == 0)) {
		PCB::running = PCB::idle;

	}
	tsp = PCB::running->sp;
	tss = PCB::running->ss;
	tbp = PCB::running->bp;
	cntr = PCB::running->quantum;

asm {

	mov sp, tsp
	mov ss, tss
	mov bp,tbp
}
}
else
{
dispatch_lockFlag=1;
if (!cntr) cntr=1;

}
}



if(!context_switch_on_demand)
asm int 60h;
context_switch_on_demand = 0;
}
void c() {
while (1) {

};

}
;

void inic() {
asm {
cli
push es
push ax

mov ax,0
mov es,ax // es = 0


mov ax, word ptr es:0022h
mov word ptr oldTimerSEG, ax
mov ax, word ptr es:0020h
mov word ptr oldTimerOFF, ax


mov word ptr es:0022h, seg timer
mov word ptr es:0020h, offset timer


mov ax, oldTimerSEG
mov word ptr es:0182h, ax
mov ax, oldTimerOFF
mov word ptr es:0180h, ax

pop ax
pop es
sti
}
PCB::list= new List();
PCB::waiting=new List();
PCB::idle= new PCB(0,1024,1,c);

//PCB::main_thread=new PCB(0,1024,0,PCB::wrapper);
//PCB::main_thread->ready=1;
//PCB::running=PCB::main_thread;
};

void restore() {
asm {
cli
push es
push ax

mov ax,0
mov es,ax

mov ax, word ptr oldTimerSEG
mov word ptr es:0022h, ax
mov ax, word ptr oldTimerOFF
mov word ptr es:0020h, ax

pop ax
pop es
sti
}

};
void Thread::blockSignal(SignalId sig) {

	if (this->myPCB->handler_list->head==0) return;
Handler_Node * temp = this->myPCB->handler_list->head;
while (temp) {
if (temp->signal_num == sig)
{

temp->blocked = 1;

}

temp=temp->next;
}

}
void Thread::blockSignalGlobally(SignalId sig) {
PCB::SignalState[sig] = 1;
}
void Thread::unblockSignal(SignalId sig) {
Handler_Node * temp = this->myPCB->handler_list->head;
while (temp) {
if (temp->signal_num == sig)
{
if (temp->blocked!=2)
temp->blocked = 0;
return;
}
temp=temp->next;
}
}
void Thread::unblockSignalGlobally(SignalId sig) {
PCB::SignalState[sig] = 0;
}
void Thread::signal(SignalId sig) {

this->myPCB->signal_list->addSignal(sig);
}
void Thread::unregisterAllHandlers(SignalId sig) {
Handler_Node* temp = this->myPCB->handler_list->head;
Handler_Node* old = 0;
Handler_Node* help;
while (temp) {
if (temp->signal_num == sig) {
	if (this->myPCB->handler_list->head==temp) this->myPCB->handler_list->head=this->myPCB->handler_list->head->next;
if (old != 0) {
old->next = temp->next;
}
help = temp;
temp = temp->next;
delete help;

}
temp = temp->next;
}

}
void Thread::registerHandler(SignalId sig, SignalHandler handle) {

this->myPCB->handler_list->addHandler(sig, handle);
}
;
void Thread::swap(SignalId sig, SignalHandler hand1, SignalHandler hand2) {
Handler_Node* temp = this->myPCB->handler_list->head;
Handler_Node* temp2 = this->myPCB->handler_list->head;
while ((temp) && (temp->body != hand1)) {
temp = temp->next;
}
if (!temp)
return;
if (temp->signal_num != sig)
return;
while ((temp2) && (temp2->body != hand2)) {
temp2 = temp2->next;
}
if (!temp2)
return;
if (temp2->signal_num != sig)
return;
SignalHandler help;
help = temp->body;
temp->body = temp2->body;
temp2->body = help;
}
;
void do_signals()
{

	lockFlag=1;
	asm sti;
	Signal_Node * temp= PCB::running->signal_list->head;
	Signal_Node * old=0;
    Signal_Node * help=0;
     while (temp!=0)
     {

    	 Handler_Node * temph=PCB::running->handler_list->head;
    	 PCB::running->signal_list->head=PCB::running->signal_list->head->next;
    	old=temp;

    	 if (temp->signal_num==0) {d();
    	 lockFlag =0;asm cli; return;}
    	while(temph)
    	{
         if(temph->signal_num==temp->signal_num) {

        	 if((!temph->blocked)&&(PCB::SignalState[temph->signal_num]!=1))
        	 {
        		 temph->body();
        	}
         }
         temph=temph->next;

    	}

    	old->next=temp->next;
    	help=temp;
    	temp=temp->next;
    	delete help;

     }
     asm cli;
lockFlag=0;


}
