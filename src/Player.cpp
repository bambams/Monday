#include "Debug.h"
#include "Player.h"
#include "Game.h"

#include <iostream>


Player::Player()
{
	up = false;
	down = false;
	left = false;
	right = false;
	height = 1;
}


void Player::Event(ALLEGRO_EVENT event)
{
	if (event.type == ALLEGRO_EVENT_KEY_DOWN)
	{
		Keypress(event.keyboard.keycode);
	}
	if (event.type == ALLEGRO_EVENT_KEY_UP)
	{
		Keyrelease(event.keyboard.keycode);
	}

	Vector vel;
	if (up)
	{
		vel += Vector(0, -1);
	}
	if (down)
	{
		vel += Vector(0, 1);
	}
	if (left)
	{
		vel += Vector(-1, 0);
	}
	if (right)
	{
		vel += Vector(1, 0);
	}

	Set_movement(vel);
}


void Player::Keypress(int keycode)
{
	assert(game != NULL && "Player::Keypress(): *game == NULL.\n");
	Entity_manager *manager = NULL;

	if (game != NULL)
	{
		manager = game->Get_entity_manager();
	}

	if (keycode == ALLEGRO_KEY_UP)
	{
		up = true;
	}
	if (keycode == ALLEGRO_KEY_DOWN)
	{
		down = true;
	}
	if (keycode == ALLEGRO_KEY_LEFT)
	{
		left = true;
	}
	if (keycode == ALLEGRO_KEY_RIGHT)
	{
		right = true;
	}
	if (keycode == ALLEGRO_KEY_A)
	{
		if (manager != NULL)
		{
			manager->Pick_up(position, 100, this, true);
		}
		std::cout << "Inventory has " << this->Number_of_entities() << " entities" << std::endl;
	}
	if (keycode == ALLEGRO_KEY_Z)
	{
		if (manager != NULL)
		{
			manager->Interact(position, 100, this);
		}
		std::cout << "Inventory has " << this->Number_of_entities() << " entities" << std::endl;
	}
}


void Player::Keyrelease(int keycode)
{
	if (keycode == ALLEGRO_KEY_UP)
	{
		up = false;
	}
	if (keycode == ALLEGRO_KEY_DOWN)
	{
		down = false;
	}
	if (keycode == ALLEGRO_KEY_LEFT)
	{
		left = false;
	}
	if (keycode == ALLEGRO_KEY_RIGHT)
	{
		right = false;
	}
}


void Player_register_lua_callbacks(lua_State* state)
{
}
