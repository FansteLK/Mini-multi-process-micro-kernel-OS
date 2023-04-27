
#include "KernelS.h"
#include "SCHEDULE.H"
#include "PCB.H"
extern volatile unsigned int lockFlag ;
extern   volatile int context_switch_on_demand ;
extern   volatile int dispatch_lockFlag;
void dispatch();
	  KernelSem::KernelSem(int value)
		{
	   this->value=value;
	   this->sem_blocked=new List();
		}
	  KernelSem::~KernelSem()
	  	{
	  		this->sem_blocked->deleteList();

	  	}
	  int KernelSem::block(Time t)
	  {


		  PCB::running->blocked=1;
		  PCB::running->semaphore=this;


		  if(t!=0)
		  {
		  PCB::waiting->addPCBsort((PCB*)PCB::running,t);
		  }
		  this->sem_blocked->addPCB((PCB*)PCB::running);
          lockFlag=0;
		  dispatch();

		  return PCB::running->timer_signal;
	  }
	  int KernelSem::unblock(int s)
	  {


		  PCB_Node * temp=this->sem_blocked->head;
		  int retval=0;
		  PCB_Node * old;
		  if (temp){
		  if(s==0)
		  {

			  temp->elem->blocked=0;
			  temp->elem->timer_signal=1;

			  Scheduler::put(temp->elem);
			  PCB::waiting->deletePCBsort(temp->elem->This_id);
			sem_blocked->deletePCB( temp->elem->This_id);
			  temp=0;
			  return 0;
		  }
		  else
		  {

           while ((temp)&&(s>0))
           {

        	  retval++;
        	   s--;


            temp->elem->blocked=0;
        	temp->elem->timer_signal=1;
        	 old=temp;
        	Scheduler::put(temp->elem);

        	temp=temp->next;
        	PCB::waiting->deletePCBsort(old->elem->This_id);
        	        	sem_blocked->deletePCB(old->elem->This_id);




           }
           return retval;
		  }
		  	  }
		  return 0;
	  }
