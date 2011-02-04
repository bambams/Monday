#include "Container.h"
#include "Debug.h"
#include "Entity.h"
#include "Game.h"

#include <algorithm>


Container::Container() :
	game(NULL)
{
}


void Container::Set_game(Game* igame)
{
	game = igame;
}


Game* Container::Get_game()
{
	return game;
}


Container::~Container()
{
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->Destroy();
	}
}


void Container::Add_entity(Entity* e)
{
	mon_assert(NULL != e);
	entities.push_back(e);
}


void Container::Remove_entity(Entity* e)
{
	mon_assert(NULL != e);
	removals.push_back(e);
}


int Container::Number_of_entities()
{
	return entities.size();
}


Entities& Container::Get_entities()
{
	return entities;
}


void Container::Render(const Camera& camera)
{
	Process_removals();
	std::sort(entities.begin(), entities.end());
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->Render(camera);
	}
}


/**
 * Function: Pick_up
 *
 * Pick up an item (all items are Entities) and add it to your inventory.
 *
 * Parameters:
 *   from: Position of the Entity (Player) picking up the item
 *   r:    Radius around item to determine whether the Entity is close enough
 *   to:   Pointer to Entity (Player) whose inventory receives the item
 *   all:  Endless_supply defines
 */
void Container::Pick_up(Vector from, float r, Container* to, bool all)
{
	Process_removals();
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		Vector diff = from - (*i)->Get_position();
		float length = diff.Length();
		if (length <= r)
		{
			if ((*i)->Pick_up(to) && !all)
				break;
		}
	}
}


void Container::Interact(Vector from, float r, Container* to)
{
	Process_removals();
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		Vector diff = from - (*i)->Get_position();
		float length = diff.Length();
		if (length <= r)
		{
			if ((*i)->Interact(to))
				break;
		}
	}
}


void Container::Process_removals()
{
	for (Entities::iterator i = removals.begin(); i != removals.end(); ++i)
	{
		entities.erase(std::find(entities.begin(), entities.end(), *i));
	}
	removals.clear();
}
