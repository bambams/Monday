#ifndef Player_h
#define Player_h

#include "Animation.h"
#include "Animator.h"
#include "Vector.h"
extern "C"
{
	#include "lua.h"
}
void Player_register_lua_callbacks(lua_State* state);

class Player
{
public:
	Player();
	void Initiate();
	void Update(double dt);
	void Render();
	void Event(ALLEGRO_EVENT event);
	void Set_position(Vector pos);
	Vector Get_position();
private:
	void Keypress(int keycode);
	void Keyrelease(int keycode);

	Vector position;
	bool up;
	bool down;
	bool left;
	bool right;
	int height;
	Animator animator;
	Animation animation;
};

#endif
