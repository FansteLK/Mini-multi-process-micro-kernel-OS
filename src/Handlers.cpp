/*
 * Handlers.cpp
 *
 *  Created on: Aug 21, 2020
 *      Author: OS1
 */

#include "Handlers.h"
#include "PCB.h"
#include "SCHEDULE.H"
#include <stdio.h>
void d() {
	PCB_Node * tempera=PCB::running->blocked_list->head;
	PCB_Node * old=0;
	    while(tempera!=0)
	    {

	    	tempera->elem->blocked=0;
	    	Scheduler::put(tempera->elem);
	    	tempera=tempera->next;


	    }
	    if(PCB::running->MakerThread!=0)
	    PCB::running->MakerThread->signal(1);
	if(PCB::running->MyThread!=0)
		PCB::running->MyThread->signal(2);

	PCB::running->finished=1;


}
Handlers::Handlers() {
	this->head=0;

}
void Handlers::addHandler(unsigned num,SignalHandler body)
{

	Handler_Node* new_Handler_Node = new Handler_Node;
	new_Handler_Node->signal_num = num;
	new_Handler_Node->blocked=0;
	new_Handler_Node->next = 0;
	new_Handler_Node->body=body;
	if (this->head == 0) {

		this->head = new_Handler_Node;
	} else {
		Handler_Node* temp = head;
		while (temp->next != 0) {
			temp = temp->next;
		}
		temp->next = new_Handler_Node;

	}

}
void Handlers::deleteHandler()
{

}
void Handlers::deleteHandlers()
{
	Handler_Node* temp = head;
		Handler_Node* next = head->next;
		while (temp != 0) {
			next = temp->next;

			delete temp;

			temp = 0;
			temp = next;
		}
		head = 0;
	}
Handlers::~Handlers() {
	deleteHandlers();
}
