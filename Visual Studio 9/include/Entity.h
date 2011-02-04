#ifndef Entity_h
#define Entity_h


#include "Animator.h"
#include "Camera.h"
#include "Container.h"
#include "Entity_manager.h"
#include "Main.h"
#include "Map.h"
#include "Obstacle.h"
#include "Tileset.h"
#include "Vector.h"

#include <fstream>
#include <map>
#include <string>


class Game;

/* Class: Entity
 * Handles a scriptable object.
 */
class Entity:
        public Container
        ,public Obstacle
{
	public:
		/* Constructor: Entity
		 */
		Entity(Map* inMap = NULL);

		/* Destructor: Entity
		 */
		virtual ~Entity();

		/* Function: Load
		 * Parameters:
		 * data - A filestream to load from.
		 */
		void Load(std::ifstream& data);

		/* Function: Set_animations
		 * Sets a set of animations for this entity.
		 */
		void Set_animations(Animations& animations);

		/* Operator: operator<
		 * Compares entities by y position.
		 */
		bool operator<(const Entity& o) const;

		/* Function: Set_position
		 */
		void Set_position(Vector position);

		/* Function Get_position
		 */
		Vector Get_position();

		/* Function: Set_movement
		 * Sets a movement vector, entity will continue moving in the same direction until a new vector is set.
		 */
		void Set_movement(Vector direction);

		/* Function: Set_movement_speed
		 * Set entity's base movement speed.
		 */
		void Set_movement_speed(float speed);

		/* Function: Get_movement_speed
		 * Get entity's base movement speed.
		 */
		float Get_movement_speed();

		/* Function: Set_speed_boost
		 * Set temporary movement speed enhancement.
		 */
		void Set_speed_boost(float boost);

		/* Function: Get_speed_boost
		 * Get entity's temporary speed enhancement.
		 */
		float Get_speed_boost();

		/* Function: Set_solid
		 * Decides whether the entity can be collided with.
		 **/
		void Set_solid(const bool &isolid);

		/* Function: Is_solid
		 * Returns:
		 * The state of this entity's collidableness
		 */
		bool Is_solid();

		/* Function: Set_script
		 * Sets which script function to call upon a given event.
		 *
		 * Parameters:
		 * event - Which event to affect, accept the <Script> enums in string form.
		 * function - Which script function to set
		 */
		void Set_script(std::string event, std::string function);

		/* Function: Pick_up
		 * Executes script PICK_UP if any is set.
		 *
		 * Parameters:
		 * to - A container to put entities in if anything is picked up.
		 *
		 * Returns:
		 * true if there was a pick up script
		 */
		bool Pick_up(Container* to);

		/* Function: Interact
		 * Executes script Interact if any is set.
		 *
		 * Parameters:
		 * to - A container to put entities in if anything is picked up.
		 *
		 * Returns:
		 * true if there was an interact script
		 */
		bool Interact(Container* to);

		/* Function: Attack
		 * Go all kung-fu on an opponent
		 */
		void Attack();

		/* Function: Update
		 * Updates the entity, calling script UPDATE is any is set.
		 *
		 * Parameters:
		 * dt - Time passed since last update call.
		 */
		void Update(double dt);

		/* Function: Render
		 * Renders the entity (can be overridden by derived classes).
		 */
		virtual void Render(const Camera& camera);

		/* Function: Destroy
		 * Mark the entity for destruction. It is purged by the <Entity_manager> next update.
		 */
		void Destroy();

		/* Function: Destroyed
		 * Returns:
		 * true if the entity is marked for destruction.
		 */
		bool Destroyed();

		/* Function: Line_collision
		 * For collision handling
		 */
		virtual bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data);

		/* Function: Circle_collision
		 * For collision handling
		 */
		virtual bool Circle_collision(const Vector& center, float radius, Collision_data& data);

		/* Function: Point_blocked
		 * For collision handling
		 */
		virtual bool Point_blocked(const Vector& position);

		/* Function: Set_Dimensions
		 * Entities know about the tile dimensions in the current map so the
		 * Lua wrappers can properly position them.
		 *
		 * TODO: I don't think we should have the Entity remember any sort of
		 * tile dimensions: it should get it from the Map.
		 */
		void Set_Dimensions(const int& tilew, const int& tileh);

		float Get_Size();
		
		void Set_Name(const std::string& ent_name);
		std::string Get_Name();

	protected:
		/* Enum: Script
			PICK_UP - On pick up
			INTERACT - On Interact
			ATTACK - On attack
			UPDATE - On update
		*/
		enum Script
		{
			PICK_UP,
			INTERACT,
			ATTACK,
			UPDATE,
		};
		typedef std::map<Script, std::string> Scripts;

		Map* map;
		int tile_w, tile_h;
		Vector position;
		Vector direction;
		float size;
		Scripts scripts;
		Animator animator;
		bool destroyed;
		bool solid;
		float movement_speed;
		float speed_boost;

		std::string name;
};

void Entity_register_lua_callbacks(lua_State* state);


#endif  //  Entity_h
