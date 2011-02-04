#include "Debug.h"
#include "Entity.h"
#include "Game.h"
#include "main.h"

#include <iostream>


Entity::Entity()
	: position(0, 0),
	  direction(0, 0)
{
	destroyed = false;
	size = 12;

	movement_speed = 60.0;
	speed_boost = 0;
}


Entity::~Entity()
{
}


void Entity::Set_animations(Animations &animations)
{
	animator.Set_animations(animations);
}


bool Entity::operator < (const Entity &o) const
{
	return (position.Y() < o.position.Y());
}


void Entity::Set_position(const Vector &pos)
{
	position = pos;
}


Vector Entity::Get_position()
{
	return position;
}


void Entity::Set_movement(const Vector &idirection)
{
	direction = idirection;

	bool up = direction.Y() < 0;
	bool down = direction.Y() > 0;
	bool left = direction.X() < 0;
	bool right = direction.X() > 0;

	if ((down && !up) && (right != left))
	{
		animator.Set_animation("downright", left);
	}
	else if ((up && !down) && (right != left))
	{
		animator.Set_animation("upright", left);
	}
	else if (right != left)
	{
		animator.Set_animation("right", left);
	}
	else if (!up && down)
	{
		animator.Set_animation("down");
	}
	else if (up && !down)
	{
		animator.Set_animation("up");
	}
}


void Entity::Set_movement_speed(float speed)
{
	movement_speed = speed;
}


float Entity::Get_movement_speed()
{
	return movement_speed;
}


void Entity::Set_speed_boost(float boost)
{
	speed_boost = boost;
}


float Entity::Get_speed_boost()
{
	return speed_boost;
}


void Entity::Set_script(std::string s, std::string f)
{
	Script script = SCRIPT_NONE;
	if (s == "PICK_UP")
		script = PICK_UP;
	if (s == "INTERACT")
		script = INTERACT;
	if (s == "ATTACK")
		script = ATTACK;
	if (s == "UPDATE")
		script = UPDATE;

	if (script != SCRIPT_NONE)
	{
		scripts[script] = f;
	}
	else
	{
		scripts[script] = "";
	}
}


bool Entity::Pick_up(Container *to)
{
	if (game != NULL)
	{
		Lua_wrapper *wrapper = game->Get_lua_wrapper();
		if (wrapper != NULL)
		{
			lua_State *state = wrapper->Prepare(scripts[PICK_UP], this);
			if (state != NULL)
			{
				if (scripts[PICK_UP] != "")
				{
					lua_pushlightuserdata(state, to);
					wrapper->Call(1);
					return true;
				}
				else
				{
					std::cerr << "Entity::Pick_up(): scripts[PICK_UP] == \"\".\n";
				}
			}
			else
			{
				std::cerr << "Entity::Pick_up(): wrapper->Prepare(scripts[PICK_UP], this) returned NULL.\n";
				return false;
			}
		}
		else
		{
			std::cerr << "Entity::Pick_up(): game->Get_lua_wrapper() returned NULL.\n";
			return false;
		}
	}
	else
	{
		std::cerr << "Entity::Pick_up(): *game == NULL.\n";
	}

	return false;
}


bool Entity::Interact(Container *to)
{
	if (game != NULL)
	{
		Lua_wrapper *wrapper = game->Get_lua_wrapper();
		if (wrapper != NULL)
		{
			lua_State *state = wrapper->Prepare(scripts[INTERACT], this);
			if (state != NULL)
			{
				if (scripts[INTERACT] != "")
				{
					lua_pushlightuserdata(state, to);
					wrapper->Call(1);
					return true;
				}
				else
				{
					std::cerr << "Entity::Interact(): scripts[INTERACT] == \"\".\n";
				}
			}
			else
			{
				std::cerr << "Entity::Interact(): wrapper->Prepare(scripts[INTERACT], this) returned NULL.\n";
			}
		}
		else
		{
			std::cerr << "Entity::Interact(): game->Get_lua_wrapper() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity::Interact(): *game == NULL.\n";
	}

	return false;
}


void Entity::Attack()
{
	if (game != NULL)
	{
		Lua_wrapper *wrapper = game->Get_lua_wrapper();
		if (wrapper != NULL)
		{
			wrapper->Call(scripts[ATTACK], this);
		}
		else
		{
			std::cerr << "Entity::Attack(): game->Get_lua_wrapper() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity::Attack(): *game == NULL.\n";
	}
}


void Entity::Update(double dt)
{
	if (game != NULL)
	{
		Lua_wrapper *wrapper = game->Get_lua_wrapper();
		if (wrapper != NULL)
		{
			lua_State *state = wrapper->Prepare(scripts[UPDATE], this);
			if (state != NULL)
			{
				std::cerr << "Entity::Update(): wrapper->Prepare(scripts[UPDATE], this) returned NULL.\n";
				lua_pushnumber(state, dt);
				wrapper->Call(1);
			}

			Vector newposition = position + direction * dt * (movement_speed + speed_boost);
			Collision_data collision_data;

			Obstacle_manager *manager = game->Get_obstacle_manager();
			if (manager != NULL)
			{
				if (manager->Circle_collision(newposition, size, collision_data))
				{
					position = newposition + collision_data.adjust;
				}
				else
				{
					position = newposition;
				}
			}
			else
			{
				std::cerr << "Entity::Update(): game->Get_obstacle_manager() returned NULL.\n";
			}
			animator.Update(dt);
		}
		else
		{
			std::cerr << "Entity::Update(): game->Get_lua_wrapper() returned NULL.\n";
			return;
		}
	}
	else
	{
		std::cerr << "Entity::Update(): *game is NULL.  This isn't good...\n";
		return;
	}
}


void Entity::Render(const Camera *camera)
{
	if (camera != NULL)
	{
		Vector rpos = camera->Apply(position);
		animator.Render(rpos, 1);
		al_draw_rectangle(rpos.X() - size, rpos.Y() - size, rpos.X() + size, rpos.Y() + size, al_map_rgb(100, 0, 0), 0);
//		al_draw_circle(rpos.X(), rpos.Y(), 8, al_map_rgb(100, 0, 0), 0);
	}
	else
	{
		std::cerr << "Entity::Render(): *camera is NULL.\n";
	}
}


void Entity::Destroy()
{
	destroyed = true;
}


bool Entity::Destroyed()
{
	return destroyed;
}


bool Entity::Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data)
{
	//TODO: Collide with circle
	return false;
}


bool Entity::Circle_collision(const Vector &center, float radius, Collision_data &data)
{
	//TODO: Fill in collision data
	if ((center - position).Length() < size + radius)
	{
		return true;
	}

	return false;
}


bool Entity::Point_blocked(const Vector &iposition, const Vector &from)
{
	//TODO: Fill in directional collision detection based off of "from" vector
	if ((iposition - position).Length() < size)
	{
		return true;
	}
	return false;
}


/*
 * Lua bindings
*/
int Entity_set_position(lua_State *state)
{
	assert(state != NULL && "Entity_set_position(): *state == NULL.\n");

	Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
	assert(entity != NULL && "Entity_set_position(): static_cast<Entity *>(lua_touserdata(state, 1)) returned NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	assert(game != NULL && "Entity_set_position(): *game == NULL.\n");

	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			manager->Assert_entity(entity);
			
			int x = lua_tonumber(state, 2);
			int y = lua_tonumber(state, 3);
			
			entity->Set_position(Vector(x, y));
		}
		else
		{
			std::cerr << "Entity_set_position(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity_set_position(): Lua_wrapper::Get_game() returned NULL.\n";
	}

	return 0;
}


int Entity_set_movement(lua_State *state)
{
	assert(state != NULL && "Entity_set_movement(): *state == NULL.\n");

	Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
	assert(entity != NULL && "Entity_set_movement(): static_cast<Entity *>(lua_touserdata(state, 1)) returned NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	assert(game != NULL && "Entity_set_movement(): *game == NULL.\n");

	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			manager->Assert_entity(entity);

			int x = lua_tonumber(state, 2);
			int y = lua_tonumber(state, 3);

			if (entity != NULL)
			{
				entity->Set_movement(Vector(x, y));
			}
		}
		else
		{
			std::cerr << "Entity_set_movement(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity_set_movement(): Lua_wrapper::Get_game() returned NULL.\n";
	}

	return 0;
}


int Entity_get_position(lua_State *state)
{
	assert(state != NULL && "Entity_get_position(): *state == NULL.\n");

	Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
	assert(entity != NULL && "Entity_get_position(): static_cast<Entity *>(lua_touserdata(state, 1)) returned NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			manager->Assert_entity(entity);
			
			if (entity != NULL)
			{
				Vector position = entity->Get_position();

				lua_pushnumber(state, position.X());
				lua_pushnumber(state, position.Y());
				
				/* Two items pushed to LUA stack */
				return 2;
			}
		}
		else
		{
			std::cerr << "Entity_get_position(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity_get_position(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	/* Something was wrong: nothing pushed to stack */
	return 0;
}


int Entity_set_script(lua_State *state)
{
	assert(state != NULL && "Entity_set_script(): *state == NULL.\n");

	Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
	assert(entity != NULL && "Entity_set_script(): static_cast<Entity *>(lua_touserdata(state, 1)) returned NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			manager->Assert_entity(entity);

			const char *s = lua_tostring(state, 2);
			const char *f = lua_tostring(state, 3);
			if (entity != NULL)
			{
				entity->Set_script(s, f);
			}
			std::cout << "Script: " << s << " Function: " << f << std::endl;
		}
		else
		{
			std::cerr << "Entity_set_script(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity_set_script(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	return 0;
}


int Entity_set_animations(lua_State *state)
{
	assert (state != NULL && "Entity_set_animations(): *state == NULL.\n");

	Entity *entity = static_cast<Entity *>(lua_touserdata(state, 1));
	assert(entity != NULL && "Entity_set_animations(): static_cast<Entity *>(lua_touserdata(state, 1)) returned NULL.\n");

	Game *game = Lua_wrapper::Get_game(state);
	if (game != NULL)
	{
		Entity_manager *manager = game->Get_entity_manager();
		if (manager != NULL)
		{
			manager->Assert_entity(entity);
			
			const char *s = lua_tostring(state, 2);
			Resource_manager *manager = game->Get_resource_manager();
			if (manager != NULL)
			{
				Animations animations = Load_animations(s, manager);
			
				if (entity != NULL)
				{
					entity->Set_animations(animations);
				}
			}
			else
			{
				std::cerr << "Entity_set_animations(): game->Get_resource_manager() returned NULL.\n";
			}
		}
		else
		{
			std::cerr << "Entity_set_animations(): game->Get_entity_manager() returned NULL.\n";
		}
	}
	else
	{
		std::cerr << "Entity_set_animations(): Lua_wrapper::Get_game(state) returned NULL.\n";
	}

	return 0;
}


void Entity_register_lua_callbacks(lua_State *state)
{
	assert(state != NULL && "Entity_register_lua_callbacks(): *state == NULL.\n");
	if (state != NULL)
	{
		lua_register(state, "Entity_set_position", Entity_set_position);
		lua_register(state, "Entity_get_position", Entity_get_position);
		lua_register(state, "Entity_set_movement", Entity_set_movement);
		lua_register(state, "Entity_set_script", Entity_set_script);
		lua_register(state, "Entity_set_animations", Entity_set_animations);
	}
}
