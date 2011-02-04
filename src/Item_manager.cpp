#include "Item_manager.h"

Item_manager::~Item_manager()
{
	for(Items::iterator i = items.begin(); i!=items.end(); ++i)
		delete i->second;
}

void Item_manager::Add(Item* item, Vector position)
{
	items.push_back(std::pair<Vector,Item*>(position, item));
}

Item* Item_manager::Remove(Vector position)
{
	for(Items::iterator i = items.begin(); i!=items.end(); ++i)
	{
		Vector diff = position - i->first;
		if(diff.Length()<32)
		{
			Item* item = i->second;
			items.erase(i);
			return item;
		}
	}
	return NULL;
}

void Item_manager::Render()
{
	for(Items::iterator i = items.begin(); i!=items.end(); ++i)
	{
		i->second->Render(i->first);
	}
}
