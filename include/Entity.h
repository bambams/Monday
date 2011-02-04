#ifndef Entity_h
#define Entity_h

#include "Vector.h"
#include "Container.h"
#include "Animator.h"
#include "Camera.h"
#include "Entity_manager.h"
#include "Obstacle.h"
#include <map>
#include <string>

extern "C"
{
	#include "lua.h"
}

class Game;

enum Script
{
	PICK_UP,
	INTERACT,
	ATTACK,
	UPDATE,
};

typedef std::map<Script, std::string> Scripts;

class Entity: public Container, public Obstacle
{
public:
	Entity();
	virtual ~Entity();
	void Set_animations(Animations& animations);
	bool operator<(const Entity& o) const;
	void Set_position(Vector position);
	Vector Get_position();
	void Set_movement(Vector direction);

	void Set_script(std::string s, std::string f);
	bool Pick_up(Container* to);
	bool Interact(Container* to);
	void Attack();
	void Update(double dt);

	void Render(const Camera& camera);
	
	void Destroy();
	bool Destroyed();

	virtual bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data);
	virtual bool Circle_collision(const Vector& center, float radius, Collision_data& data);
	virtual bool Point_blocked(const Vector& position);
protected:
	Vector position;
	Vector direction;
	float size;
	Scripts scripts;
	Animator animator;
	bool destroyed;
};

void Entity_register_lua_callbacks(lua_State* state);

#endif
