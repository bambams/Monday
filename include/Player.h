#ifndef Player_h
#define Player_h

#include "Animation.h"
#include "Animator.h"
#include "Vector.h"
#include "Entity.h"
#include "Camera.h"
#include "Camera.h"
extern "C"
{
	#include "lua.h"
}
void Player_register_lua_callbacks(lua_State* state);

class Game;

class Player: public Entity
{
public:
	Player();
	void Event(ALLEGRO_EVENT event);
private:
	void Keypress(int keycode);
	void Keyrelease(int keycode);

	bool up;
	bool down;
	bool left;
	bool right;
	int height;
};

#endif
