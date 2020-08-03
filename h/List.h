
#ifndef _LIST_H_
#define _LIST_H_
#include <iostream.h>
class PCB;
class KernelSem;
struct PCB_Node
{
	PCB * elem;
	PCB_Node* next;
	 int time_wait;
	KernelSem* sem;
};
 class List
{
 public:

void print(){
	PCB_Node* t = head;
	while(t){
		cout<<t->time_wait<<endl;
		t=t->next;
	}
	cout <<"///////////////\n";
}

PCB_Node * head;
List();
void addPCBsort(PCB* pcb,int t=0);
void deletePCBsort(int idf);
void decPCB();
void addPCB(PCB* pcb,int t=0);
void deletePCB(int idf);
PCB* getPCB(int idf);
void deleteList();
~List();
};


#endif
