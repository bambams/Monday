#ifndef Entity_manager_h
#define Entity_manager_h

#include "Camera.h"
#include "Container.h"
#include "Main.h"
#include "Vector.h"

#include <map>
#include <string>
#include <vector>

class Entity;
class Game;
class Player;
class Map;

typedef std::map<Entity*, Container*> Contained_entities;

/*
 * Class: Entity_manager
 * Responsible for ensuring proper management of all entities.
 */
class Entity_manager
{
	public:
		/* Constructor: Entity_manager
		 */
		Entity_manager();

		/* Destructor: Entity_manager
		 */
		~Entity_manager();

		/* Function: Set_player
		 * Tells the manager about the Player entity.
		 */
		void Set_player(Player* player);

		/* Function: Set_game
		 * Tells the manager about it's game instance.
		 */
		void Set_game(Game* game);

		/* Function: Create_entity
		 * Creates and initializes a new entity
		 *
		 * Parameters:
		 * container - Which container to put the entity in.
		 *
		 * Returns:
		 * The created entity.
		 */
		Entity* Create_entity(Container* container);

		/* Function: Destroy_entity
		 * Destroys an entity and makes sure it's cleaned out of the system.
		 */
		void Destroy_entity(Entity* entity);

		/* Function: Update
		 * Parameters:
		 * dt - Time passed since last update.
		 */
		void Update(double dt);

		/* Function: Transfer_entity
		 * Moves an entity from one container to another.
		 *
		 * Parameters:
		 * entity - The entity to move.
		 * to - Container to move it to.
		 */
		void Transfer_entity(Entity* entity, Container* to);

		/* Function: Assert_entity
		 * Make sure the entity pointer is valid.
		 */
		void Assert_entity(Entity* entity);

		/* Function: Assert_container
		 * Make sure the container is valid.
		 */
		void Assert_container(Container* container);

	private:
		//All entities, mapped to their containers
		Contained_entities contained_entities;
		Player* player;
		Game* game;
};

void Entity_manager_register_lua_callbacks(lua_State* state);

#endif
