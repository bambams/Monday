#include "Character.h"
#include "Game.h"
#include <allegro5/allegro5.h>
extern "C"
{
#include <lua.h>
}
#include <iostream>


Character::Character()
{
}

void Character::Set_update_script(std::string function)
{
	update_script = function;
}

void Character::Update(double dt, Game* game)
{
	if(update_script=="")
		return;
	lua_State* L = game->Get_lua_wrapper().Get_state();
	lua_getglobal(L, update_script.c_str());
	lua_pushlightuserdata(L, this);
	lua_pushnumber(L, dt);
	int errors = lua_pcall(L, 2, 0, 0);

//	lua_getglobal(L, "Test");
//	lua_pcall(L, 0, 0, 0);
//	int errors = lua_pcall(L, 0, 0, 0);

	if (errors)
	{
		std::cout << lua_tostring(L, -1) << std::endl;
	}

	/* move to dest */
	Vector delta = destination-position;
	float l = delta.Length();	
	if (l < speed) position = destination; /*generate 'arrived' event*/ return;
	if (l > 0) { 
		position += delta *speed /l ;
	}
}

void Character::Set_position(Vector pos)
{
	position = pos;
}

void Character::Set_destination(Vector dest)
{
	destination = dest;
}

void Character::Set_speed(float s)
{
	speed = s;
}


void Character::Render()
{
	al_draw_rectangle(position.X()-5, position.Y()-5, position.X()+5, position.Y()+5, al_map_rgb(100, 0, 0), 0);
}

/* LUA callbacks */
int Character_set_position(lua_State* state)
{
//	assert(lua_gettop(state) == 2);

	Character* character = static_cast<Character*>(lua_touserdata(state, 1));

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	character->Set_position(Vector(x, y));
	character->Set_speed(0.1);

	//printf("[LUA] Character_set_position\n");

	return 0;
}

int Character_set_destination(lua_State* state)
{
//	assert(lua_gettop(state) == 2);

	Character* character = static_cast<Character*>(lua_touserdata(state, 1));

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	character->Set_destination(Vector(x, y));
	character->Set_speed(0.1);

	//printf("[LUA] Character_set_destination\n");

	return 0;
}

void Character_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Character_set_position", Character_set_position);
	lua_register(state, "Character_set_destination", Character_set_position);
}
