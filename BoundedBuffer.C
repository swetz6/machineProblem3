#include "BoundedBuffer.H"

BoundedBuffer::BoundedBuffer(int _size){
	buf = new Item[_size];
	count = 0;
	usedSlots = Semaphore(0);
	openSlots = Semaphore(_size);
	itemsTakenOut = 0;
	itemsPutIn=0;
}

BoundedBuffer::BoundedBuffer(){
	buf = new Item[0];
	count = 0;
	usedSlots = Semaphore(0);
	openSlots = Semaphore(0);
	itemsTakenOut = 0;
	itemsPutIn = 0;
}

void BoundedBuffer::insert_item(Item* item){
	openSlots.P();
	buf[count] = *item;
	count++;
	itemsPutIn++;
	int osV = usedSlots.V();
}

Item * BoundedBuffer::get_item(){
	count = usedSlots.P();
	Item *item = &buf[count];
	itemsTakenOut++; 
	openSlots.V();
	return item;
}
