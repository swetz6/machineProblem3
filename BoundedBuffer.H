
#ifndef boundedbuffer_H_
#define boundedbuffer_H_

#include <string>
#include <stdlib.h>
#include <iostream>
#include "Item.H"
#include "Semaphore.H"

class BoundedBuffer {
	Item *buf;		
	int count, itemsTakenOut, itemsPutIn;
	Semaphore openSlots;
	Semaphore usedSlots;
public:
	BoundedBuffer(int _size);
	BoundedBuffer();
	void insert_item(Item* item);	
	Item * get_item();
	int numItemsPutIn(){return itemsPutIn;}
	int numItemsTakenOut(){return itemsTakenOut;}
};


#endif 
