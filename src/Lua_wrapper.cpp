#include "Lua_wrapper.h"
#include <iostream>

Lua_wrapper::Lua_wrapper()
:state(NULL)
{
}

Lua_wrapper::~Lua_wrapper()
{
	if(state)
		lua_close(state);
}

void Lua_wrapper::Init()
{
	state = lua_open();
	luaL_openlibs(state);
}

void Lua_wrapper::Deinit()
{
	lua_close(state);
	state=NULL;
}

void Lua_wrapper::Load_script(std::string filename, bool call)
{
	if (luaL_loadfile(state, filename.c_str()) == 0)
	{
		if(call)
		{
			int errors = lua_pcall(state, 0, 0, 0);
			if (errors)
			{
				std::cout << lua_tostring(state, -1) << std::endl;
			}
		}
	}
	else
	{
		std::cout << filename <<" failed to execute" << std::endl;
	}
}

lua_State* Lua_wrapper::Get_state()
{
	return state;
}
