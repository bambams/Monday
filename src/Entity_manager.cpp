#include "Entity_manager.h"
#include <algorithm>
#include "Game.h"
#include "Entity.h"
#include <iostream>
#include "Debug.h"

Entity_manager::Entity_manager()
:player(NULL)
{
}

Entity_manager::~Entity_manager()
{
}

void Entity_manager::Set_player(Player* iplayer)
{
	player = iplayer;
}

Entity* Entity_manager::Create_entity()
{
	Entity* entity = new Entity();
	entity->Set_game(game);
	Add_entity(entity);
	contained_entities[entity] = this;
	game->Get_obstacle_manager().Add_obstacle(entity);
	return entity;
}

void Entity_manager::Destroy_entity(Entity* entity)
{
	assert(entity);
	contained_entities[entity]->Remove_entity(entity);
	contained_entities.erase(entity);
	game->Get_obstacle_manager().Remove_obstacle(entity);
	delete entity;
}

void Entity_manager::Update(double dt)
{
	for(Contained_entities::iterator i = contained_entities.begin(); i!=contained_entities.end(); ++i)
	{
		if(i->first->Destroyed())
		{
			delete i->first;
			contained_entities.erase(i);
		}
		else
		{
			i->first->Update(dt);
		}
	}
}

void Entity_manager::Transfer_entity(Entity* entity, Container* to)
{
	assert(entity && to);
	contained_entities[entity]->Remove_entity(entity);
	to->Add_entity(entity);
	contained_entities[entity] = to;
	if(to == this)
	{
		game->Get_obstacle_manager().Add_obstacle(entity);
	}
	else
	{
		game->Get_obstacle_manager().Remove_obstacle(entity);
	}
}

void Entity_manager::Assert_entity(Entity* entity)
{
	assert(entity == player || contained_entities.find(entity) != contained_entities.end());
}

void Entity_manager::Assert_container(Container* container)
{
	Entity* entity = static_cast<Entity*>(container);
	assert(container == this || container == player || contained_entities.find(entity) != contained_entities.end());
}

/*
 * Lua bindings
*/

int Destroy_entity(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));
	manager.Destroy_entity(entity);
	return 0;
}

int Create_entity(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	lua_pushlightuserdata(state, manager.Create_entity());
	return 1;
}

int Transfer_entity(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();

	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));
	manager.Assert_entity(entity);
	Container* to = static_cast<Container*>(lua_touserdata(state, 2));
	manager.Assert_container(to);

	manager.Transfer_entity(entity, to);
	return 0;
}

int Get_entity_manager(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	lua_pushlightuserdata(state, &manager);
	return 1;
}

void Entity_manager_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Create_entity", Create_entity);
	lua_register(state, "Destroy_entity", Destroy_entity);
	lua_register(state, "Transfer_entity", Transfer_entity);
	lua_register(state, "Get_entity_manager", Get_entity_manager);
}
