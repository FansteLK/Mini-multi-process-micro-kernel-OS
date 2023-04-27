#include "List.h"
#include "PCB.h"
#include "SCHEDULE.H"
extern volatile unsigned int lockFlag;
extern volatile int context_switch_on_demand;
extern volatile int dispatch_lockFlag;
void dispatch();
List::List() {
	head = 0;
}
;
void List::decPCB() {

	if(head==0) return;

	PCB_Node* temp = head;
	PCB_Node* old = temp;
	temp->time_wait=temp->time_wait-1;

	while( (temp->time_wait == 0)&&(temp!=0)) {

      head=temp->next;
		PCB_Node* old = temp;
		temp->elem->timer_signal = 0;
		temp->elem->blocked = 0;
		Scheduler::put(temp->elem);
		temp->elem->semaphore->sem_blocked->deletePCB(temp->elem->This_id);
    old->next=0;
	delete old;
		temp = head;



	}

}
void List::addPCBsort(PCB* pcb,  int t) {

	//pravljenje cvora

	PCB_Node* new_PCB_Node = new PCB_Node;

	new_PCB_Node->elem = pcb;
	new_PCB_Node->next = 0;
	new_PCB_Node->time_wait = t;
	//
	//Ako je prazna
	if (this->head == 0) {
		this->head = new_PCB_Node;
		return;
	}
	//
	//old je prosla za temp

	else {

		PCB_Node* temp = head;
		PCB_Node* old = temp;
		//Manja od prvog
		if (new_PCB_Node->time_wait < temp->time_wait) {
			temp->time_wait = temp->time_wait - new_PCB_Node->time_wait;
			new_PCB_Node->next = temp;
			head = new_PCB_Node;

			return;
		} else {

			//new_PCB_Node->time_wait = new_PCB_Node->time_wait - temp->time_wait;
			while ((temp->time_wait <= new_PCB_Node->time_wait) && (temp->next)) {
				new_PCB_Node->time_wait = new_PCB_Node->time_wait
						- temp->time_wait;
				old = temp;
				temp = temp->next;

			}
			if (!temp->next) {

				if (temp->time_wait <= new_PCB_Node->time_wait) {
					new_PCB_Node->time_wait = new_PCB_Node->time_wait- temp->time_wait;
					temp->next = new_PCB_Node;

					return;

				} else {
					temp->time_wait=temp->time_wait-new_PCB_Node->time_wait;
					if(old!=0)
					old->next = new_PCB_Node;
					new_PCB_Node->next = temp;

					return;
				}
			} else {
				temp->time_wait=temp->time_wait-new_PCB_Node->time_wait;
				new_PCB_Node->next = temp;
				if(old!=0)
				old->next = new_PCB_Node;

               return;
			}
		}

	}

}
void List::addPCB(PCB* pcb,  int t) {

	PCB_Node* new_PCB_Node = new PCB_Node;
	new_PCB_Node->elem = pcb;
	new_PCB_Node->next = 0;
	new_PCB_Node->time_wait = t;

	if (this->head == 0) {

		this->head = new_PCB_Node;
	} else {
		PCB_Node* temp = head;
		while (temp->next != 0) {
			temp = temp->next;
		}
		temp->next = new_PCB_Node;

	}

}
;
void List::deletePCBsort(int idf) {

	if (this->head == 0)
		return;
	PCB_Node* temp = head;
	PCB_Node* last = 0;
	while ((temp->next != 0) && (temp->elem->This_id != idf)) {
		last = temp;
		temp = temp->next;
	}
	if (temp->elem->This_id != idf)
		return;
	if (last == 0) {
		if(temp->next){
		temp->next->time_wait = head->time_wait + temp->next->time_wait;

		this->head = temp->next;

		delete temp;
		}
		else
		{
			delete temp;
			this->head=0;
		}


	} else {

		if(temp->next){
		temp->next->time_wait = temp->next->time_wait + temp->time_wait;

		last->next = temp->next;

		delete temp;
		}
		else
		{
			delete temp;
			last->next=0;

		}
	}

}
void List::deletePCB(int idf) {
	if (head == 0)
		return;
	PCB_Node* temp = head;
	PCB_Node* last = 0;
	while ((temp->next != 0) && (temp->elem->This_id != idf)) {
		last = temp;
		temp = temp->next;
	}
	if (temp->elem->This_id != idf)
		return;
	if (last == 0) {
		this->head = temp->next;

		delete temp;
		temp = 0;

	} else {
		last->next = temp->next;
		delete temp;
		temp = 0;
	}

}
;
PCB* List::getPCB(int idf) {
	PCB_Node* temp = head;
	while ((temp->next != 0) && (temp->elem->This_id != idf)) {
		temp = temp->next;
	}
	if (temp->elem->This_id == idf) {
		return temp->elem;
	}
	return 0;

}
;
void List::deleteList() {
	PCB_Node* temp = head;
	PCB_Node* next = head->next;
	while (temp != 0) {
		next = temp->next;

		delete temp;

		temp = 0;
		temp = next;
	}
	head = 0;
}
;
List::~List() {
	deleteList();
}
;

