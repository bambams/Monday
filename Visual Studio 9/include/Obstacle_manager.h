#ifndef Obstacle_manager_h
#define Obstacle_manager_h

#include <list>
#include "Vector.h"
class Obstacle;
class Collision_data;
typedef std::list<Obstacle*> Obstacles;

/* Class: Obstacle_manager
 * Handles obstacles
 * */
class Obstacle_manager
{
public:
	/* Function: Add_obstacle
	 * */
	void Add_obstacle(Obstacle* obstacle);
	/* Function: Remove_obstacle
	 * */
	void Remove_obstacle(Obstacle* obstacle);
	/* Function: Line_collision
	 * Goes through all obstacles and provides collision data to the caller.
	 * 
	 * Returns:
	 * True if anything was hit.
	 * */
	bool Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data);
	/* Function: Circle_collision
	 * Goes through all obstacles and provides collision data to the caller.
	 * 
	 * Returns:
	 * True if anything was hit.
	 * */
	bool Circle_collision(const Vector& center, float radius, Collision_data& data, Obstacle* self);
	/* Function: Point_blocked
	 * Checks with all obstacles to see if a point is blocked.
	 * */
	bool Point_blocked(const Vector& position);
private:
	Obstacles obstacles;
};

#endif
