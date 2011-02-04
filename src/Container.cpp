#include "Container.h"
#include "Entity.h"
#include "Debug.h"
#include "Game.h"
#include <algorithm>

Container::Container()
	: game(NULL)
{
	// Nothing to do
}


void Container::Set_game(Game *igame)
{
	game = igame;
}


Container::~Container()
{
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->Destroy();
	}
}


void Container::Add_entity(Entity *e)
{
	assert(e && "Container::Add_entity: *e is NULL.\n");
	if (e != NULL)
	{
		entities.push_back(e);
	}
}


void Container::Remove_entity(Entity *e)
{
	assert(e && "Container::Remove_entity: *e is NULL.\n");
	if (e != NULL)
	{
		removals.push_back(e);
	}
}


int Container::Number_of_entities()
{
	return entities.size();
}


Entities &Container::Get_entities()
{
	return entities;
}


void Container::Render(const Camera *camera)
{
	Process_removals();
	std::sort(entities.begin(), entities.end());
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		(*i)->Render(camera);
	}
}


void Container::Pick_up(Vector from, float r, Container *to, bool all)
{
	Process_removals();
	for (Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		Vector diff = from - (*i)->Get_position();
		float length = diff.Length();
		if (length <= r)
		{
			if ((*i)->Pick_up(to) && !all)
			{
				break;
			}
		}
	}
	Process_removals();
}


void Container::Interact(Vector from, float r, Container *to)
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
	Process_removals();
}


void Container::Process_removals()
{
	for (Entities::iterator i = removals.begin(); i != removals.end(); ++i)
	{
		entities.erase(std::find(entities.begin(), entities.end(), *i));
	}
	removals.clear();
}
