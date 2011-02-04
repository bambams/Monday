#ifndef Entity_manager_h
#define Entity_manager_h


#include "main.h"
#include "Camera.h"
#include "Container.h"
#include "Vector.h"

#include <vector>
#include <map>
#include <string>

class Entity;
class Game;
class Player;

typedef std::map<Entity *, Container *> Contained_entities;


class Entity_manager: public Container
{
	public:
		Entity_manager();
		~Entity_manager();
		void Set_player(Player *player);
		Entity *Create_entity();
		void Destroy_entity(Entity *entity);
		void Update(double dt);
		void Transfer_entity(Entity *entity, Container *to);

		void Assert_entity(Entity *entity);
		void Assert_container(Container *container);

	private:
		//All entities, mapped to their containers
		Contained_entities contained_entities;
		Player *player;
};

void Entity_manager_register_lua_callbacks(lua_State *state);


#endif  //  Entity_manager_h
