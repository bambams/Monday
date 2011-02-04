#ifndef Obstacle_h
#define Obstacle_h

#include "Vector.h"

class Obstacle;

/* Class: Collision_data
 * Information about what was collided with.
 * */
class Collision_data
{
public:
	Vector adjust;
	Obstacle* obstacle;
private:
};

/* Class: Obstacle
 * Base class for objects you can collide with
 * */
class Obstacle
{
public:
	/* Destructor: Obstacle
	 * */
	virtual ~Obstacle();
	/* Function: Line_collision
	 * For finding obstacles along a line.
	 * */
	virtual bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data)=0;
	/* Function: Circle_collision
	 * */
	virtual bool Circle_collision(const Vector& center, float radius, Collision_data& data)=0;
	/* Function: Point_blocked
	 * For knowing if a certain point if blocked.
	 * */
	virtual bool Point_blocked(const Vector& position)=0;
private:
};

#endif
