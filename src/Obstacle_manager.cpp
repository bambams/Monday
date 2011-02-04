#include "Obstacle_manager.h"
#include "Debug.h"
#include "Obstacle.h"
#include <algorithm>

void Obstacle_manager::Add_obstacle(Obstacle *obstacle)
{
	assert(obstacle);
	obstacles.push_back(obstacle);
}


void Obstacle_manager::Remove_obstacle(Obstacle *obstacle)
{
	assert(obstacle);
	obstacles.erase(std::find(obstacles.begin(), obstacles.end(), obstacle));
}



//TODO: Using collision data, find first collision if several are found...
bool Obstacle_manager::Line_collision(const Vector &line_begin, const Vector &line_end, Collision_data &data)
{
	for (Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if ((*i)->Line_collision(line_begin, line_end, data))
		{
			return true;
		}
	}
	return false;
}


bool Obstacle_manager::Circle_collision(const Vector &center, float radius, Collision_data &data)
{
	for (Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if ((*i)->Circle_collision(center, radius, data))
		{
			return true;
		}
	}
	return false;
}


bool Obstacle_manager::Point_blocked(const Vector &position)
{
	for (Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if (Point_blocked(position))
		{
			return true;
		}
	}
	return false;
}
