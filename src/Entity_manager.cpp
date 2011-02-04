#include "Entity_manager.h"
#include "Debug.h"
#include "Entity.h"
#include "Game.h"

#include <algorithm>
#include <iostream>


Entity_manager::Entity_manager()
	: player(NULL)
{
	// Default constructor
}


Entity_manager::~Entity_manager()
{
	if (player != NULL)
	{
		delete player;
		player = NULL;
	}
}


void Entity_manager::Set_player(Player *iplayer)
{
	player = iplayer;
}


Entity *Entity_manager::Create_entity()
{
	Entity *entity = new Entity();
	if (entity != NULL)
	{
		entity->Set_game(game);
		Add_entity(entity);
		contained_entities[entity] = this;
	}
	else
	{
		std::cerr << "Entity_manager::Create_entity(): new Entity() returned NULL.\n";
	}

	return entity;
}


void Entity_manager::Destroy_entity(Entity *entity)
{
	assert(entity != NULL && "Entity_manager::Destroy_entity(): *entity == NULL.\n");

	if (entity != NULL)
	{
		contained_entities[entity]->Remove_entity(entity);
		contained_entities.erase(entity);
		delete entity;
		entity = NULL;
	}
}


void Entity_manager::Update(double dt)
{
	for (Contained_entities::iterator i = contained_entities.begin(); i != contained_entities.end(); ++i)
	{
		if (i->first->Destroyed())
		{
			delete i->first;
			contained_entities.erase(i);
		}
		i->first->Update(dt);
	}
}


void Entity_manager::Transfer_entity(Entity *entity, Container *to)
{
	assert(entity != NULL && to != NULL);
	contained_entities[entity]->Remove_entity(entity);
	if (to != NULL)
	{
		to->Add_entity(entity);
		contained_entities[entity] = to;
	}
}


void Entity_manager::Assert_entity(Entity *entity)
{
	assert(entity && (entity == player || contained_entities.find(entity) != contained_entities.end() ));
}


void Entity_manager::Assert_container(Container *container)
{
	Entity *entity = static_cast<Entity *>(container);
	assert(container && (container == this || container == player || contained_entities.find(entity) != contained_entities.end()));
}


/*
 * Lua bindings
*/

int Destroy_entity(lua_State *state)
{
	assert (state != NULL && "Destroy_entity(): *state == NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
			manager->Destroy_entity(entity);
		}
		else
		{
			std::cerr << "Destroy_entity(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Destroy_entity(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	return 0;
}


int Create_entity(lua_State *state)
{
	assert (state != NULL && "Create_entity(): *state == NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			lua_pushlightuserdata(state, manager->Create_entity());
			return 1;
		}
		else
		{
			std::cerr << "Create_entity(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Create_entity(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	/* Could not push onto the LUA stack: error */
	return 0;
}


int Transfer_entity(lua_State *state)
{
	assert (state != NULL && "Create_entity(): *state == NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
			manager->Assert_entity(entity);
			
			Container *to = static_cast<Container *>(lua_touserdata(state, 2));
			manager->Assert_container(to);
			
			manager->Transfer_entity(entity, to);
		}
		else
		{
			std::cerr << "Transfer_entity(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Transfer_entity(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	return 0;
}


int Get_entity_manager(lua_State *state)
{
	Game *game = Lua_wrapper::Get_game(state);
	Entity_manager *manager = game->Get_entity_manager();
	lua_pushlightuserdata(state, &manager);
	return 1;
}


void Entity_manager_register_lua_callbacks(lua_State *state)
{
	lua_register(state, "Create_entity", Create_entity);
	lua_register(state, "Destroy_entity", Destroy_entity);
	lua_register(state, "Transfer_entity", Transfer_entity);
	lua_register(state, "Get_entity_manager", Get_entity_manager);
}
