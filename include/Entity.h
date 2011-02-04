#ifndef Entity_h
#define Entity_h

#include "main.h"
#include "Animator.h"
#include "Camera.h"
#include "Container.h"
#include "Entity_manager.h"
#include "Obstacle.h"
#include "Vector.h"

#include <map>
#include <string>


class Game;

enum Script
{
	PICK_UP = 0,
	INTERACT,
	ATTACK,
	UPDATE,

	SCRIPT_NONE
};

typedef std::map<Script, std::string> Scripts;


class Entity
	: public Container,
	  public Obstacle
{
	public:
		Entity();
		virtual ~Entity();

		void Set_animations(Animations &animations);
		bool operator<(const Entity &o) const;
		void Set_position(const Vector &pos);
		Vector Get_position();

		void Set_movement(const Vector &dir);
		void Set_movement_speed(float speed);
		float Get_movement_speed();

		void Set_speed_boost(float boost);
		float Get_speed_boost();
	
		void Set_script(std::string s, std::string f);
		bool Pick_up(Container *to);
		bool Interact(Container *to);
		void Attack();
		void Update(double dt);
	
		void Render(const Camera *camera);
		
		void Destroy();
		bool Destroyed();
	
		virtual bool Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data);
		virtual bool Circle_collision(const Vector &center, float radius, Collision_data &data);
		virtual bool Point_blocked(const Vector &position, const Vector &from);

	protected:
		Vector position;
		Vector direction;
		Scripts scripts;
		Animator animator;

		bool destroyed;
		float movement_speed;
		float speed_boost;
		float size;
};

void Entity_register_lua_callbacks(lua_State *state);


#endif  //  Entity_h
