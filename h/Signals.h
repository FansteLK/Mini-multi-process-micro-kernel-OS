/*
 * Signals.h
 *
 *  Created on: Aug 21, 2020
 *      Author: OS1
 */

#ifndef BCC_SIGNALS_H_
#define BCC_SIGNALS_H_
struct Signal_Node
{
	unsigned signal_num;
	Signal_Node* next;

};
class Signals {
public:
	Signal_Node * head;
	Signals();
	void deleteSignal();
	void addSignal(unsigned num);
	void deleteList();
	virtual ~Signals();
};

#endif /* BCC_SIGNALS_H_ */
