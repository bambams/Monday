#include "Entity_manager.h"

#include "Debug.h"
#include "Entity.h"
#include "Game.h"

#include <algorithm>
#include <iostream>


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


void Entity_manager::Set_game(Game* igame)
{
	game = igame;
}


Entity* Entity_manager::Create_entity(Container* container)
{
	Entity* entity = new Entity(container);
	entity->Set_game(game);

	container->Add_entity(entity);
	contained_entities[entity] = container;

	game->Get_lua_wrapper().Call("Initialize_entity", entity);

	return entity;
}


void Entity_manager::Destroy_entity(Entity* entity)
{
	mon_assert(entity);
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
	mon_assert(entity && to);
	contained_entities[entity]->Remove_entity(entity);
	to->Add_entity(entity);
	contained_entities[entity] = to;
}


void Entity_manager::Assert_entity(Entity* entity)
{
	mon_assert(entity == player || contained_entities.find(entity) != contained_entities.end());
}


void Entity_manager::Assert_container(Container* container)
{
	//Todo: Fix
	//Entity* entity = static_cast<Entity*>(container);
	//mon_assert(container == player || contained_entities.find(entity) != contained_entities.end());
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
	Container* container = static_cast<Container*>(lua_touserdata(state, 1));
	manager.Assert_container(container);
	lua_pushlightuserdata(state, manager.Create_entity(container));
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

int Get_container_entities(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();

	Container* container = static_cast<Container*>(lua_touserdata(state, 1));
	manager.Assert_container(container);

	Entities& entities = container->Get_entities();
	int p = 0;
	lua_newtable(state);
	for(Entities::iterator i = entities.begin(); i != entities.end(); ++i)
	{
		++p;
		lua_pushinteger(state, p);
		lua_pushlightuserdata(state, *i);
		lua_settable (state, -3);
	}
	return 1;
}

/*
int Get_entity_manager(lua_State* state)
{
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	lua_pushlightuserdata(state, &manager);
	return 1;
}
*/
void Entity_manager_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Create_entity", Create_entity);
	lua_register(state, "Destroy_entity", Destroy_entity);
	lua_register(state, "Transfer_entity", Transfer_entity);
	lua_register(state, "Get_container_entities", Get_container_entities);
	//Obsolete
//	lua_register(state, "Get_entity_manager", Get_entity_manager);
}
