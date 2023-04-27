/*
 * Signals.cpp
 *
 *  Created on: Aug 21, 2020
 *      Author: OS1
 */

#include "Signals.h"
#include <iostream.h>
Signals::Signals() {
	this->head=0;
}
void Signals::addSignal(unsigned num)
{

	Signal_Node* new_Signal_Node = new Signal_Node;
	new_Signal_Node->signal_num = num;
	new_Signal_Node->next = 0;
	if (this->head == 0) {

		this->head = new_Signal_Node;
	} else {
		Signal_Node* temp = head;
		while (temp->next != 0) {
			temp = temp->next;
		}
		temp->next = new_Signal_Node;

	}

}
void Signals::deleteSignal()
{
	}
void Signals::deleteList()
{
	Signal_Node* temp = head;
	Signal_Node* next = head->next;
	while (temp != 0) {
		next = temp->next;

		delete temp;

		temp = 0;
		temp = next;
	}
	head = 0;
}
Signals::~Signals() {
	deleteList();
}

