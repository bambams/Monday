#ifndef Player_h
#define Player_h


#include "Animation.h"
#include "Animator.h"
#include "Camera.h"
#include "Entity.h"
#include "Main.h"
#include "Vector.h"


void Player_register_lua_callbacks(lua_State* state);

class Game;

/* Class: Player
 * Handling the player avatar
 */
class Player:
	public Entity
{
	public:
		/* Constructor: Player
		 */
		Player();

		/* Function: Event
		 * Handles events coming from the game.
		 */
		void Event(ALLEGRO_EVENT event);

		/* Function: Render
		 * Renders the player (overrides from Entity's Render())
		 */
		virtual void Render(const Camera& camera);

	private:
		void Keypress(int keycode);
		void Keyrelease(int keycode);

		bool up;
		bool down;
		bool left;
		bool right;
		int height;
};


#endif  // Player_h
