#ifndef Obstacle_h
#define Obstacle_h

#include "Vector.h"

class Obstacle;

class Collision_data
{
public:
	Vector adjust;
	Obstacle* obstacle;
private:
};

class Obstacle
{
public:
	virtual ~Obstacle();
	virtual bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data)=0;
	virtual bool Circle_collision(const Vector& center, float radius, Collision_data& data)=0;
	virtual bool Point_blocked(const Vector& position)=0;
private:
};

#endif
