#include "Entity.h"
#include "Game.h"
#include <iostream>

Entity::Entity()
:size(12)
,destroyed(false)
{
}

Entity::~Entity()
{
}

void Entity::Set_animations(Animations& animations)
{
	animator.Set_animations(animations);
}

bool Entity::operator<(const Entity& o) const
{
	return position.Y()<o.position.Y();
}

void Entity::Set_position(Vector pos)
{
	position = pos;
}

Vector Entity::Get_position()
{
	return position;
}

void Entity::Set_movement(Vector idirection)
{
//	std::cout<<"Movement: "<<idirection.X()<<" "<<idirection.Y()<<std::endl;
	direction = idirection;

	bool up = direction.Y()<0;
	bool down = direction.Y()>0;
	bool left = direction.X()<0;
	bool right = direction.X()>0;

	if((down && !up) && (right != left))
		animator.Set_animation("downright", left);
	else if((up && !down) && (right != left))
		animator.Set_animation("upright", left);
	else if(right != left)
		animator.Set_animation("right", left);
	else if(!up && down)
		animator.Set_animation("down");
	else if(up && !down)
		animator.Set_animation("up");
}

void Entity::Set_script(std::string s, std::string f)
{
	Script script;
	if(s == "PICK_UP")
		script = PICK_UP;
	if(s == "INTERACT")
		script = INTERACT;
	if(s == "ATTACK")
		script = ATTACK;
	if(s == "UPDATE")
		script = UPDATE;
	scripts[script] = f;
}

bool Entity::Pick_up(Container* to)
{
	lua_State* state = game->Get_lua_wrapper().Prepare(scripts[PICK_UP], this);
	if(!state || scripts[PICK_UP]=="")
		return false;
	lua_pushlightuserdata(state, to);
	game->Get_lua_wrapper().Call(1);
	return true;
}

bool Entity::Interact(Container* to)
{
	lua_State* state = game->Get_lua_wrapper().Prepare(scripts[INTERACT], this);
	if(!state || scripts[INTERACT]=="")
		return false;
	lua_pushlightuserdata(state, to);
	game->Get_lua_wrapper().Call(1);
	return true;
}

void Entity::Attack()
{
	game->Get_lua_wrapper().Call(scripts[ATTACK], this);
}

void Entity::Update(double dt)
{
	lua_State* state = game->Get_lua_wrapper().Prepare(scripts[UPDATE], this);
	if(state)
	{
		lua_pushnumber(state, dt);
		game->Get_lua_wrapper().Call(1);
	}

	Vector newposition = position+direction*dt*50;
	Collision_data collision_data;
	if(game->Get_obstacle_manager().Circle_collision(newposition, size, collision_data, this))
	{
		position = newposition + collision_data.adjust;
	}
	else
	{
		position = newposition;
	}
	animator.Update(dt);
}

void Entity::Render(const Camera& camera)
{
	Vector rpos = camera.Apply(position);
	animator.Render(rpos, 1);
	al_draw_rectangle(rpos.X()-size, rpos.Y()-size, rpos.X()+size, rpos.Y()+size, al_map_rgb(100, 0, 0), 0);
//	al_draw_circle(rpos.X(), rpos.Y(), 8, al_map_rgb(100, 0, 0), 0);
}

void Entity::Destroy()
{
	destroyed = true;
}

bool Entity::Destroyed()
{
	return destroyed;
}

bool Entity::Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data)
{
	//TODO: Collide with circle
	return false;
}

bool Entity::Circle_collision(const Vector& center, float radius, Collision_data& data)
{
	Vector difference = center-position;
	float length = difference.Length();
	if(length < size+radius)
	{
		data.obstacle = this;
		difference.Normalize();
		data.adjust = difference*(size+radius-length);
		return true;
	}
	return false;
}

bool Entity::Point_blocked(const Vector& iposition)
{
	if((iposition-position).Length() < size)
		return true;
	return false;
}

/*
 * Lua bindings
*/
int Entity_set_position(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	entity->Set_position(Vector(x, y));

	return 0;
}

int Entity_set_movement(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	int x = lua_tonumber(state, 2);
	int y = lua_tonumber(state, 3);

	entity->Set_movement(Vector(x, y));

	return 0;
}

int Entity_get_position(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	Vector position = entity->Get_position();

	lua_pushnumber(state, position.X());
	lua_pushnumber(state, position.Y());

	return 2;
}

int Entity_set_script(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	const char* s = lua_tostring(state, 2);
	const char* f = lua_tostring(state, 3);
	entity->Set_script(s, f);
	std::cout<<"Script: "<<s<<" Function: "<<f<<std::endl;
	return 0;
}

int Entity_set_animations(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	const char* s = lua_tostring(state, 2);

	Animations animations = Load_animations(s, game->Get_resource_manager());
	entity->Set_animations(animations);
	return 0;
}

void Entity_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Entity_set_position", Entity_set_position);
	lua_register(state, "Entity_get_position", Entity_get_position);
	lua_register(state, "Entity_set_movement", Entity_set_movement);
	lua_register(state, "Entity_set_script", Entity_set_script);
	lua_register(state, "Entity_set_animations", Entity_set_animations);
}
