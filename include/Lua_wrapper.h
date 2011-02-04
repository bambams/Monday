#ifndef Lua_wrapper_h
#define Lua_wrapper_h

#include <string>
extern "C"
{
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}

class Lua_wrapper
{
public:
	Lua_wrapper();
	~Lua_wrapper();
	void Deinit();
	void Init();
	void Load_script(std::string filename, bool call=true);
	lua_State* Get_state();
private:
	lua_State* state;
};

#endif
