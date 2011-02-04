#ifndef Container_h
#define Container_h


#include "Main.h"
#include "Camera.h"

#include <vector>


class Game;
class Entity;
typedef std::vector<Entity*> Entities;

/*	Class: Container
 *	Handles a set of entities.
 */
class Container
{
	public:
		/* Constructor: Container
		 */
		Container();

		/* Destructor: Container
		 */
		virtual ~Container();

		/* Function: Set_game
		 * Sets game instance that this container operates in.
		 */
		void Set_game(Game* game);

		/* Function: Get_game
		 * Returns the game paired up with the current container
		 */
		Game* Get_game();

		/* Function: Add_entity
		 * Adds an entity to the container
		 */
		virtual void Add_entity(Entity* e);

		/* Function: Remove_entity
		 * Removes an entity from the container.
		 */
		virtual void Remove_entity(Entity* e);

		/*
			Function: Number_of_entities

			Returns:
				Number of entities in container
		*/
		int Number_of_entities();

		/* Function: Render
		 * Renders the entities of his container
		 */
		void Render(const Camera& camera);

		/*
			Function: Pick_up
				Finds entities in reach and asks them to execute pick up script.

			Parameters:
				from - Center of your range.
				r - Radius of your range.
				to - Container to put any picked up entities in
				all - pick up all entities in reach or just one
		*/
		void Pick_up(Vector from, float r, Container* to, bool all);

		/*
			Function: Interact
				Finds entities in reach and asks them to execute interact script.

			Parameters:
				from - Center of your range.
				r - Radius of your range.
				to - Container to put entities in
		*/
		void Interact(Vector from, float r, Container* to);

		/* Function: Get_entities
		 * Returns:
		 * The list of entities contained.
		 */
		Entities& Get_entities();

	protected:
		void Process_removals();
		Game* game;

	private:
		Entities removals;
		Entities entities;
};


#endif  //  Container_h
