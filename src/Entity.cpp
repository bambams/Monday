#include "Entity.h"

#include "Debug.h"
#include "Game.h"

/* To parse config files of "name=value" pairs */
#include "Parse_Lines.h"

/* Defines Attributes, as well as u32, u16, etc. */
#include "StdTypes.h"

#include <iostream>
#include <string>
#include <vector>


Entity::Entity(Container* inContainer) :
	container(inContainer)
	,size(12)
	,destroyed(false)
	,solid(false)
{
	Map* map = dynamic_cast<Map*>(container);
	if (NULL != map)
	{
		tile_w = map->Get_TileWidth();
		tile_h = map->Get_TileHeight();
	}
	else {
		tile_w = 32; // this should be 32x32, rather than 16x16
		tile_h = 32;
	}

	movement_speed = 60.0;
	speed_boost = 0;
	
	animator.Set_animation("down");

	name = "UNSPECIFIED";
}


Entity::~Entity()
{
}


void Entity::Load(std::ifstream& data)
{
	Attributes attributes;

	std::string line;
	while (seek_non_comment_line(data, line))
	{
		if ("</entity>" == line)
		{
			/* Used to only need to type attribute name once: prevents typos */
			std::string attributeName;

			/* Specify the tile from the top-left of the map that the entity is
			 * standing on.  This aligns the entity to the top-left corner of
			 * the tile instead of the center.
			 */
			if (attributes.end() != attributes.find("tile_x") &&
				attributes.end() != attributes.find("tile_y"))
			{
				Show_attributes(std::cout, attributes);

				attributeName = "tile_x";
				int tile_x = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				attributeName = "tile_y";
				int tile_y = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				float x = tile_w * tile_x - (tile_w / 2);
				float y = tile_h * tile_y - (tile_h / 2);
				Set_position(Vector(x, y));
			}

			/* Specify the tile from the top-left of the map that the entity is
			 * standing on.  The "c" at the end means to center the entity.
			 */
			if (attributes.end() != attributes.find("tile_xc") &&
				attributes.end() != attributes.find("tile_yc"))
			{
				Show_attributes(std::cout, attributes);

				attributeName = "tile_xc";
				int tile_x = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				attributeName = "tile_yc";
				int tile_y = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				float x = tile_w * tile_x;
				float y = tile_h * tile_y;
				Set_position(Vector(x, y));
			}

			/* These are absolute coordinates on a map: designer needs to know
			 * the map's tile dimensions in order to line things up as expected.
			 */
			if (attributes.end() != attributes.find("x") &&
				attributes.end() != attributes.find("y"))
			{
				Show_attributes(std::cout, attributes);

				attributeName = "x";
				float x = String_to_type<float>(attributes[attributeName]);
				attributes.erase(attributeName);

				attributeName = "y";
				float y = String_to_type<float>(attributes[attributeName]);
				attributes.erase(attributeName);

				Set_position(Vector(x, y));
			}

			attributeName = "interact";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				scripts[INTERACT] = attributes[attributeName];
				attributes.erase(attributeName);
			}

			attributeName = "pick_up";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				scripts[PICK_UP] = attributes[attributeName];
				attributes.erase(attributeName);
			}

			attributeName = "update";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				scripts[UPDATE] = attributes[attributeName];
				attributes.erase(attributeName);
			}

			attributeName = "attack";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				scripts[ATTACK] = attributes[attributeName];
				attributes.erase(attributeName);
			}

			attributeName = "solid";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				Set_solid("true" == attributes[attributeName]);
				attributes.erase(attributeName);
			}

			attributeName = "animations";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				Animations animations = Load_animations(attributes[attributeName], game->Get_resource_manager());
				Set_animations(animations);
				attributes.erase(attributeName);
			}

			attributeName = "name";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);
				Set_Name(attributes[attributeName]);
				attributes.erase(attributeName);
			}

			Show_attributes(std::cout, attributes);
			//Any remaining attributes are meant for use in the lua scripts
			for (Attributes::iterator i = attributes.begin(); i != attributes.end(); ++i)
			{
				lua_State* state = game->Get_lua_wrapper().Prepare("Set_attribute", this);
				lua_pushlstring(state, i->first.c_str(), i->first.length());
				std::vector<std::string> tokens = tokenize(i->second, " \t");

				Monday_out(VERBOSE_LEVEL2, std::cout, "Token size: %d\n", tokens.size());
				for (u32 t = 0; t < tokens.size(); ++t)
				{
					Monday_out(VERBOSE_LEVEL2, std::cout, "  tokens[%d]: %s\n", t, tokens[t].c_str());
				}

				//Todo: Support other lua types
				size_t tokensSize = tokens.size() - 1;
				if (tokensSize > 0)
				{
					if ("string" == tokens[tokensSize])
					{
						/* Everything between the equals sign and the word
						 * "string" will be the string, though the words will
						 * have to be reconstructed from a tokenized state.
						 * Don't have spaces in your string, since they will be
						 * removed:
						 *   name    =   gravy     boat   string
						 * becomes:
						 *   name = gravy boat string
						 * and the final string is:
						 *   gravy boat
						 */
						std::string combined = tokens[0];
						for (size_t i = 1; i < tokensSize; ++i)
						{
							combined = combined + " " + tokens[i];
						}
						lua_pushlstring(state, combined.c_str(), combined.length());
					}
					else if ("number" == tokens[tokensSize])
					{
						lua_pushnumber(state, String_to_type<float>(tokens[0]));
					}
					else
					{
						printlog(tokens[tokensSize] + " is not a supported lua type");
					}
				}
				game->Get_lua_wrapper().Call(2);
			}

			// Once the closing tag is found, do not scan the file any further.
			break;
		}

		//Store name, value pair in attributes
		std::string name;
		std::vector<std::string> value;

		/* Ensure that the line has a valid "name=value" pair, else we will
		 * see problems.
		 */
		if (true == name_and_value(line, name, value))
		{
			/* Until we figure out how to have multiple values with the same
			 * name, we're only going to be able to take the first one...
			 */
			attributes[name] = value[0];
		}
		else
		{
			/* If this fails, the line doesn't have a "name=value" pair.
			 * It's not an error we can't recover from, so this is where a
			 * warning can be displayed, if desired.
			 */
		}

//		log(name + " = " + value);
	}
}


void Entity::Set_animations(Animations& animations)
{
	animator.Set_animations(animations);
}


bool Entity::operator < (const Entity& o) const
{
	return position.Y() < o.position.Y();
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
//	std::cout << "Movement: " << idirection.X() << " " << idirection.Y() << std::endl;
	direction = idirection;

	bool up = direction.Y()<0;
	bool down = direction.Y()>0;
	bool left = direction.X()<0;
	bool right = direction.X()>0;

	if ((down && !up) && (right != left))
		animator.Set_animation("downright", left);
	else if ((up && !down) && (right != left))
		animator.Set_animation("upright", left);
	else if (right != left)
		animator.Set_animation("right", left);
	else if (!up && down)
		animator.Set_animation("down");
	else if (up && !down)
		animator.Set_animation("up");
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
	Script script;
	if ("PICK_UP" == s)
	{
		script = PICK_UP;
	}
	else if ("INTERACT" == s)
	{
		script = INTERACT;
	}
	else if ("ATTACK" == s)
	{
		script = ATTACK;
	}
	else if ("UPDATE" == s)
	{
		script = UPDATE;
	}
	else
	{
		Monday_out(WARNING_LEVEL, std::cout, "*WARNING: Unrecognized script type: %s\n", s.c_str());
		return;
	}

	scripts[script] = f;
}


bool Entity::Pick_up(Container* to)
{
	lua_State* state = game->Get_lua_wrapper().Prepare(scripts[PICK_UP], this);
	if (!state || "" == scripts[PICK_UP])
	{
		return false;
	}
	lua_pushlightuserdata(state, to);
	game->Get_lua_wrapper().Call(1);
	return true;
}


bool Entity::Interact(Container* to)
{
	lua_State* state = game->Get_lua_wrapper().Prepare(scripts[INTERACT], this);
	if (!state || "" == scripts[INTERACT])
	{
		return false;
	}
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
	if (state)
	{
		lua_pushnumber(state, dt);
		game->Get_lua_wrapper().Call(1);
	}

	Vector newposition = position + direction * dt * (movement_speed + speed_boost);
	Collision_data collision_data;
	if (solid && game->Get_obstacle_manager().Circle_collision(newposition, size, collision_data, this))
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
	float x = rpos.X();
	float y = rpos.Y();
	al_draw_rectangle(x - size, y - size, x + size, y + size, al_map_rgb(100, 0, 0), 0);
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
	if (length < size+radius)
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
	if ((iposition-position).Length() < size)
		return true;
	return false;
}


void Entity::Set_solid(const bool &isolid)
{
	if (isolid == solid)
	{
		return;
	}
	solid = isolid;
	if (solid)
	{
		game->Get_obstacle_manager().Add_obstacle(this);
	}
	else
	{
		game->Get_obstacle_manager().Remove_obstacle(this);
	}
}


bool Entity::Is_solid()
{
	return solid;
}


void Entity::Set_Dimensions(const int& tilew, const int& tileh)
{
	tile_w = tilew;
	tile_h = tileh;
}


float Entity::Get_Size()
{
	return size;
}


void Entity::Set_Name(const std::string& ent_name)
{
	name = ent_name;
}


std::string Entity::Get_Name()
{
	return name;
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


int Entity_set_position_tile(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	Map* map = game->Get_map();
	manager.Assert_entity(entity);

	/* Default values, should be overridden */
	int tilew = 32;
	int tileh = 32;
	if (NULL != map)
	{
		tilew = map->Get_TileWidth();
		tileh = map->Get_TileHeight();
	}

	int tilex = lua_tonumber(state, 2);
	int tiley = lua_tonumber(state, 3);

	/* Centers the entity within the tile */
	int x = (tilex * tilew + tilew / 2);
	int y = (tiley * tileh + tileh / 2);

	entity->Set_position(Vector(x, y));

	return 0;
}


int Entity_set_position_name(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));
	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	Map* map = game->Get_map();
	mon_assert(NULL != map && "Game returned a NULL map");
	manager.Assert_entity(entity);

	std::string name = lua_tostring(state, 2);
	Entity* targetEntity = NULL;

	Entities entities = map->Get_entities();
	for (Entities::iterator it = entities.begin(); it != entities.end(); ++it)
	{
		if (name == (*it)->Get_Name())
		{
			targetEntity = *it;
			break;
		}
	}

	if (NULL != targetEntity)
	{
		Vector pos = targetEntity->Get_position();
		entity->Set_position(pos);
	}

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


/**
 * Function: Entity_get_position_tile
 *
 * Return the tile that the Entity is on.
 */
int Entity_get_position_tile(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	Map* map = game->Get_map();
	manager.Assert_entity(entity);
	mon_assert(NULL != map);

	Vector position = entity->Get_position();
	float tilex = position.X() / map->Get_TileWidth();
	float tiley = position.Y() / map->Get_TileHeight();
	lua_pushnumber(state, tilex);
	lua_pushnumber(state, tiley);

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
/*	std::string logmsg = std::string("Script: ")+s+" Function: "+f;
	log(logmsg.c_str());
*///	std::cout << "Script: " << s << " Function: " << f << std::endl;
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


int Entity_set_solid(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	int solid = lua_toboolean(state, 2);
	entity->Set_solid(solid);
	return 0;
}


int Entity_set_name(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	std::string name = lua_tostring(state, 2);
	entity->Set_Name(name);
	return 0;
}

int Entity_get_name(lua_State* state)
{
	Entity* entity = static_cast<Entity*>(lua_touserdata(state, 1));

	Game* game = Lua_wrapper::Get_game(state);
	Entity_manager& manager = game->Get_entity_manager();
	manager.Assert_entity(entity);

	std::string name = entity->Get_Name();
	lua_pushstring(state, name.c_str());
	return 1;
}

void Entity_register_lua_callbacks(lua_State* state)
{
	lua_register(state, "Entity_set_position", Entity_set_position);
	lua_register(state, "Entity_set_position_tile", Entity_set_position_tile);
	lua_register(state, "Entity_set_position_name", Entity_set_position_name);
	lua_register(state, "Entity_get_position", Entity_get_position);
	lua_register(state, "Entity_get_position_tile", Entity_get_position_tile);
	lua_register(state, "Entity_set_movement", Entity_set_movement);
	lua_register(state, "Entity_set_script", Entity_set_script);
	lua_register(state, "Entity_set_animations", Entity_set_animations);
	lua_register(state, "Entity_set_solid", Entity_set_solid);
	lua_register(state, "Entity_set_name", Entity_set_name);
	lua_register(state, "Entity_get_name", Entity_get_name);
}
