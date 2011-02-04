#include "Player.h"

Player::Player()
{
	up=false;
	down=false;
	left=false;
	right=false;
	height=1;
}

void Player::Initiate()
{
	animation.Load("media/Gryphon_fly_u.adf");
	animator.Set_animation(&animation);
}

void Player::Update(double dt)
{
	Vector vel;
	if(up)
		vel+=Vector(0, -1);
	if(down)
		vel+=Vector(0, 1);
	if(left)
		vel+=Vector(-1, 0);
	if(right)
		vel+=Vector(1, 0);

	position+=vel*dt*100;
	animator.Update(dt*2);
}

void Player::Render()
{
	animator.Render(position.X(), position.Y(), height);
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
}

void Player::Keypress(int keycode)
{
	if(keycode==ALLEGRO_KEY_UP)
	{
		up=true;
	}
	if(keycode==ALLEGRO_KEY_DOWN)
	{
		down=true;
	}
	if(keycode==ALLEGRO_KEY_LEFT)
	{
		left=true;
	}
	if(keycode==ALLEGRO_KEY_RIGHT)
	{
		right=true;
	}
/*
	if(keycode==ALLEGRO_KEY_A)
	{
		++height;
	}
	if(keycode==ALLEGRO_KEY_Z)
	{
		--height;
	}
*/
}

void Player::Keyrelease(int keycode)
{
	if(keycode==ALLEGRO_KEY_UP)
	{
		up=false;
	}
	if(keycode==ALLEGRO_KEY_DOWN)
	{
		down=false;
	}
	if(keycode==ALLEGRO_KEY_LEFT)
	{
		left=false;
	}
	if(keycode==ALLEGRO_KEY_RIGHT)
	{
		right=false;
	}
}

void Player::Set_position(Vector pos)
{
	position = pos;
}

Vector Player::Get_position()
{
	return position;
}


/* callback functions for lua */
int Player_set_position(lua_State* state)
{
//	assert(lua_gettop(state) == 2);

	Player* player = static_cast<Player*>(lua_touserdata(state, 1));

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	player->Set_position(Vector(x, y));

	return 0;
}

int Player_get_position(lua_State* state)
{
	Player* player = static_cast<Player*>(lua_touserdata(state, 1));

	Vector position = player->Get_position();

	lua_pushnumber(state, position.X());
	lua_pushnumber(state, position.Y());

	return 2;
}

void Player_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Player_set_position", Player_set_position);
	lua_register(state, "Player_get_position", Player_get_position);
}
