#ifndef item_H_
#define item_H_

#include <string>

class Item{
	std::string Name;
public:
	Item(std::string n)
	:Name(n){}
	Item()
	:Name(""){}
};

#endif
