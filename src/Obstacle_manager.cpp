#include "Obstacle_manager.h"

#include "Debug.h"
#include "Obstacle.h"

#include <algorithm>


typedef std::list<Collision_data> Collision_datas;

void Obstacle_manager::Add_obstacle(Obstacle* obstacle)
{
	mon_assert(obstacle);
	obstacles.push_back(obstacle);
}


void Obstacle_manager::Remove_obstacle(Obstacle* obstacle)
{
	mon_assert(obstacle);
	obstacles.erase(std::find(obstacles.begin(), obstacles.end(), obstacle));
}


//TODO: Using collision data, find first collision if several are found...

bool Obstacle_manager::Line_collision(const Vector& line_begin, const Vector& line_end, Collision_data& data)
{
	for(Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if((*i)->Line_collision(line_begin, line_end, data))
		{
			return true;
		}
	}
	return false;
}


//Todo: replace return data with adjust vector and obstacles hit?
//Collision with self bug
bool Obstacle_manager::Circle_collision(const Vector& center, float radius, Collision_data& data, Obstacle* self)
{
	Collision_datas collision_datas;
	Collision_data temp_data;
	for(Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if(*i == self)
			continue;
		if((*i)->Circle_collision(center, radius, temp_data))
		{
			//Todo: Not fill in obstacle in the obstacles themselves
			temp_data.obstacle = *i;
			collision_datas.push_back(temp_data);
		}
	}

	if (collision_datas.size() > 0)
	{
		Vector total_adjust;
		for(Collision_datas::iterator i=collision_datas.begin(); i!=collision_datas.end(); ++i)
		{
			total_adjust+=i->adjust;
		}
		Vector average = total_adjust/collision_datas.size();

		for(Collision_datas::iterator i=collision_datas.begin(); i!=collision_datas.end(); ++i)
		{
			i->obstacle->Circle_collision(center+average, radius, temp_data);
		}

		data.adjust = average;

		return true;
	}
	return false;
}


bool Obstacle_manager::Point_blocked(const Vector& position)
{
	for(Obstacles::iterator i = obstacles.begin(); i != obstacles.end(); ++i)
	{
		if(Point_blocked(position))
		{
			return true;
		}
	}
	return false;
}
