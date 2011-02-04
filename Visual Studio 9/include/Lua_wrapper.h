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

/* Class: Lua_wrapper
 * Wraps some lua functionality and common procedures.
 * */
class Lua_wrapper
{
public:
	/* Constructor: Lua_wrapper
	 * */
	Lua_wrapper();
	/* Destructor: Lua_wrapper
	 * */
	~Lua_wrapper();
	/* Function: Deinit
	 * Shuts down lua
	 * */
	void Deinit();
	/* Function: Init
	 * Initializes lua
	 * 
	 * Parameters:
	 * game - the game instance this lua wrapper belongs to.
	 * */
	void Init(Game* game);
	/* Function: Load_script
	 * Loads a script, optionally executing it.
	 * 
	 * Parameters:
	 * filename - scriptfile to load
	 * call - whether to execute the script, true by default
	 * */
	void Load_script(std::string filename, bool call=true);
	/* Function: Call
	 * Calls a lua function with no arguments except for the caller.
	 * 
	 * Parameters:
	 * function - Which lua function to call
	 * caller - Which object that called, very commonly used.
	*/
	void Call(std::string function, void* caller);
	/* Function: Prepare
	 * Pushes function global and caller on stack, and allows you to add more arguments before making the call.
	 * Use <Call(int args)> to execute the function.
	 * 
	 * Parameters:
	 * function - Which lua function to call
	 * caller - Which object that called, very commonly used.
	 *
 	 * Returns:
	 * lua_State for you to push extra arguments
	*/
	lua_State* Prepare(std::string function, void* caller);
	/* Function: Call
	 * Calls a <Prepare>d function
	 * args is the number of extra arguments you have pushed on the stack
	*/
	void Call(int args);
	/* Function: Get_state
	 * Allows access to the lua_State for doing things this wrapper doesn't wrap.
	 * 
	 * Returns:
	 * The lua_State
	 * */
	lua_State* Get_state();
	
	/* Function: Get_game
	 * A global function used to find out which game instance is coupled with a lua_State.
	 * This is used when getting calls from Lua to access the system.
	 * 
	 * Returns:
	 * the game instance this lua_State belongs to.
	*/
	static Game* Get_game(lua_State* state);
private:
	lua_State* state;
	static std::map<lua_State*, Game*> gamemap;
};

#endif
