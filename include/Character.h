#ifndef Character_h
#define Character_h

#include "Vector.h"
#include <string>
#include "Animator.h"
extern "C"
{
	#include "lua.h"
}
class Game;

class Character
{
public:
	Character();
	void Set_update_script(std::string function);
	void Update(double dt, Game* game);
	void Set_position(Vector position);
	void Set_destination(Vector d);
	void Set_speed(float s);
	void Render();
private:
	Vector position;
	Vector destination;
	float speed;
	Animator animator;
	std::string update_script;
};

void Character_register_lua_callbacks(lua_State* state);

#endif
