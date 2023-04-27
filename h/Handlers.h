/*
 * Handlers.h
 *
 *  Created on: Aug 21, 2020
 *      Author: OS1
 */

#ifndef HANDLERS_H_
#define HANDLERS_H_
typedef void (*SignalHandler)();
struct Handler_Node
{
SignalHandler body;
	unsigned signal_num;
	unsigned blocked;
	Handler_Node* next;


};
class Handlers {
public:
	Handler_Node * head;
	Handlers();
	void deleteHandler();
	void addHandler(unsigned num,SignalHandler body);
	void deleteHandlers();
	virtual ~Handlers();
};


#endif /* HANDLERS_H_ */
