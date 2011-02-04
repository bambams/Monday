#ifndef Obstacle_manager_h
#define Obstacle_manager_h


#include "Vector.h"
#include <list>

class Obstacle;
class Collision_data;

typedef std::list<Obstacle *> Obstacles;

class Obstacle_manager
{
	public:
		void Add_obstacle(Obstacle *obstacle);
		void Remove_obstacle(Obstacle *obstacle);

		bool Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data);
		bool Circle_collision(const Vector &center, float radius, Collision_data &data);
		bool Point_blocked(const Vector &position);

	private:
		Obstacles obstacles;
};


#endif  //  Obstacle_manager_h
