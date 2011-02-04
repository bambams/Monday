#include "Player.h"

#include "Debug.h"
#include "Game.h"

#include <iostream>
#include <sstream>


Player::Player() :
	Entity(NULL)
{
	up=false;
	down=false;
	left=false;
	right=false;
	height=1;
}


/**
 * Function: Event
 *
 * Handles events that come from the game.
 *
 * Currently, this function supports keyboard events, such as keydowns and
 * keyups that we need to track.
 */
void Player::Event(ALLEGRO_EVENT event)
{
	switch (event.type)
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			Keypress(event.keyboard.keycode);
			break;

		case ALLEGRO_EVENT_KEY_UP:
			Keyrelease(event.keyboard.keycode);
			break;

		default:
			break;
	}

	Vector vel;
	if(up)
		vel+=Vector(0, -1);
	if(down)
		vel+=Vector(0, 1);
	if(left)
		vel+=Vector(-1, 0);
	if(right)
		vel+=Vector(1, 0);
	Set_movement(vel);
}


void Player::Keypress(int keycode)
{
	switch (keycode)
	{
		case ALLEGRO_KEY_UP:
			up = true;
			break;

		case ALLEGRO_KEY_DOWN:
			down = true;
			break;

		case ALLEGRO_KEY_LEFT:
			left = true;
			break;

		case ALLEGRO_KEY_RIGHT:
			right = true;
			break;

		case ALLEGRO_KEY_A:
		{
			Map* map = game->Get_map();
			mon_assert(NULL != map && "Game returned a NULL map.\n");
			/* We are looking for a "radius", which I'm just fudging and taking
			 * the tile width defined in the map.  Though, to be more accurate,
			 * there should be another determiner (maybe even the average of
			 * the tile width*height).
			 */
			map->Pick_up(position, map->Get_TileWidth(), this, true);
			break;
		}

		case ALLEGRO_KEY_Z:
		{
			Map* map = game->Get_map();
			mon_assert(NULL != map && "Game returned a NULL map.\n");
			/* We are looking for a "radius", which I'm just fudging and taking
			 * the tile width defined in the map.  Though, to be more accurate,
			 * there should be another determiner (maybe even the average of
			 * the tile width*height).
			 */
			map->Interact(position, map->Get_TileWidth(), this);
			Monday_out(SUGGESTION_LEVEL, std::cout, "Collected items: %d\n", this->Get_entities().size());
			break;
		}

		default:
			break;
	}
}


void Player::Keyrelease(int keycode)
{
	switch (keycode)
	{
		case ALLEGRO_KEY_UP:
			up = false;
			break;

		case ALLEGRO_KEY_DOWN:
			down = false;
			break;

		case ALLEGRO_KEY_LEFT:
			left = false;
			break;

		case ALLEGRO_KEY_RIGHT:
			right = false;
			break;

		default:
			break;
	}
}


void Player::Render(const Camera& camera)
{
	/* Allow Entity's default Render() to be called first */
	Entity::Render(camera);

	Vector rpos = camera.Apply(Entity::position);
	float x = Entity::position.X();
	float y = Entity::position.Y();

	/* If any level of debugging is turned on, show the player's coordinates */
	if (showDebug > CRITICAL_LEVEL && showDebug < NUM_DEBUG_DEFINITIONS)
	{
		ALLEGRO_COLOR green = al_map_rgba_f(0.25, 0.75, 0.25, 1);
		ALLEGRO_COLOR black = al_map_rgba_f(0.0, 0.0, 0.0, 1);
		ALLEGRO_FONT* font = game->Get_font();
		std::stringstream ss;
		Monday_out(showDebug, ss, "(%d,%d): (%d,%d)..(%d,%d)", int(x), int(y), int(x - size), int(y - size), int(x + size), int(y + size));
		std::string positionStr = ss.str();

		/* Black, slightly offset lettering to increase visibility */
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, black);
		al_font_textout_centre(font, al_get_display_width() / 2 + 1, 12 + 1, positionStr.c_str(), -1);

		/* Green lettering */
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, green);
		al_font_textout_centre(font, al_get_display_width() / 2, 12, positionStr.c_str(), -1);

		/* Restore the blender to normal */
		al_set_blender(ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA, al_map_rgba_f(1, 1, 1, 1));
	}
}


void Player_register_lua_callbacks(lua_State* state)
{
}
