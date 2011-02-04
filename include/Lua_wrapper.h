#ifndef Lua_wrapper_h
#define Lua_wrapper_h

#include <string>
#include <map>
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class Game;

class Lua_wrapper
{
public:
	Lua_wrapper();
	~Lua_wrapper();
	void Deinit();
	void Init(Game* game);
	void Load_script(std::string filename, bool call=true);
	/*
	 * Calls a lua function with no arguments except for the caller.
	*/
	void Call(std::string function, void* caller);
	/*
	 * Prepare pushes function global and caller on stack
	 * returns state so you can push extra arguments
	 * use Call(args) to execute the function
	*/
	lua_State* Prepare(std::string function, void* caller);
	/*
	 * Calls a prepared function
	 * args is the number of extra arguments you have pushed on the stack
	*/
	void Call(int args);
	lua_State* Get_state();
	
	/*
	 * Return the game instance this lua_State belongs to.
	*/
	static Game* Get_game(lua_State* state);
private:
	lua_State* state;
	static std::map<lua_State*, Game*> gamemap;
};

#endif
