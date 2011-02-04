#ifndef Item_manager_h
#define Item_manager_h

#include "Vector.h"
#include "Item.h"
#include <vector>

typedef std::vector< std::pair<Vector, Item*> > Items;

class Item_manager
{
public:
	~Item_manager();
	void Add(Item* item, Vector position);
	Item* Remove(Vector position);
	void Render();
private:
	Items items;
};

#endif
