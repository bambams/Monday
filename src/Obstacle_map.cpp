#include "Obstacle_map.h"
#include <iostream>

Obstacle_map::Obstacle_map()
{
}

void Obstacle_map::Load(std::ifstream& data)
{
	int width;
	int height;
	data>>width;
	data>>height;

	data>>tilesize_x;
	data>>tilesize_y;

	tilemap.resize(width);
	for (int y = 0; y < height; ++y)
	{
		for (Tilemap::iterator c = tilemap.begin(); c < tilemap.end(); ++c)
		{
			char index;
			data>>index;
			c->push_back(index == '1');
		}
	}
}

bool Obstacle_map::Line_collision(const Vector& from, const Vector& to, Collision_data& data)
{
	//Todo: Fill in collision data
	
	Vector diff = to - from;
	float xslope = diff.X()/diff.Y();
	float yslope = diff.Y()/diff.X();

	float block=-1;
	
	float x=from.X();
	if(diff.X()>0)
	{
		for(x=int(x/tilesize_x)*tilesize_x+tilesize_x; x<to.X(); x+=tilesize_x)
		{
			float y = from.Y() + yslope * (x-from.X());
			if(Point_blocked(Vector(x, y)))
			{
				block = (x-from.X())/diff.X();
				break;
			}
		}
	}
	if(diff.X()<0)
	{
		for(x=int(x/tilesize_x)*tilesize_x-1; x>to.X(); x-=tilesize_x)
		{
			float y = from.Y() + yslope * (x-from.X());
			if(Point_blocked(Vector(x, y)))
			{
				block = (x-from.X())/diff.X();
				break;
			}
		}
	}
	float y=from.Y();
	if(diff.Y()>0)
	{
		for(y=int(y/tilesize_y)*tilesize_y+tilesize_y; y<to.Y(); y+=tilesize_y)
		{
			float x = from.X() + xslope * (y-from.Y());
			if(Point_blocked(Vector(x, y)))
			{
				block = (y-from.Y())/diff.Y();
				break;
			}
		}
	}
	if(diff.Y()<0)
	{
		for(y=int(y/tilesize_y)*tilesize_y-1; y>to.Y(); y-=tilesize_y)
		{
			float x = from.X() + xslope * (y-from.Y());
			if(Point_blocked(Vector(x, y)))
			{
				block = (y-from.Y())/diff.Y();
				break;
			}
		}
	}
/*	For every border x from x1 to x2
		y += yslope*(x-x1);
		control blocked

	For every border y from y1 to y2
		x += xslope*(y-y1);
		control blocked
*/
	if(block>0)
		return false;
	return true;
}

bool Obstacle_map::Circle_collision(const Vector& center, float radius, Collision_data& data)
{
	//Todo: implement
	/*
	 * From tile center is on, check if circle goes into neighbouring tiles.
	 * Diagonally, distance to corners, gives adjustment towards center?
	 * Along axis, adjustment along axis.
	*/

	//Todo: fill in collision data

	data.obstacle = this;

	int tile_x = center.X()/tilesize_x;
	int tile_y = center.Y()/tilesize_y;
	
	float ontile_x = center.X()-tile_x*tilesize_x;
	float ontile_y = center.Y()-tile_y*tilesize_y;

	bool ret=false;

	if(ontile_x-radius < 0 && Tile_blocked(tile_x-1, tile_y))
	{
		data.adjust+=Vector(radius-ontile_x, 0);
		ret = true;
	}

	if(ontile_x+radius > tilesize_x && Tile_blocked(tile_x+1, tile_y))
	{
		data.adjust+=Vector(tilesize_x - (ontile_x+radius), 0);
		ret = true;
	}

	if(ontile_y-radius < 0 && Tile_blocked(tile_x, tile_y-1))
	{
		data.adjust+=Vector(0, radius-ontile_y);
		ret = true;
	}

	if(ontile_y+radius > tilesize_y && Tile_blocked(tile_x, tile_y+1))
	{
		data.adjust+=Vector(0, tilesize_y - (ontile_y+radius));
		ret = true;
	}
	
	if(ret)
	{
		return true;
	}

	float length;
	length = Vector(ontile_x, ontile_y).Length();
	if(length < radius && Tile_blocked(tile_x-1, tile_y-1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(ontile_x * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tilesize_x, ontile_y).Length();
	if(length < radius  && Tile_blocked(tile_x+1, tile_y-1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(-(tilesize_x-ontile_x) * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tilesize_x, ontile_y - tilesize_y).Length();
	if(length < radius && Tile_blocked(tile_x+1, tile_y+1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(-(tilesize_x-ontile_x) * percent, -(tilesize_y-ontile_y) * percent);
		return true;
	}

	length = Vector(ontile_x, ontile_y - tilesize_y).Length();
	if(length < radius && Tile_blocked(tile_x-1, tile_y+1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(ontile_x * percent, -(tilesize_y-ontile_y) * percent);
		return true;
	}

	return false;
}

bool Obstacle_map::Point_blocked(const Vector& position)
{
	unsigned int x = position.X()/tilesize_x;
	unsigned int y = position.Y()/tilesize_y;
	return Tile_blocked(x, y);
}

bool Obstacle_map::Tile_blocked(unsigned int x, unsigned int y)
{
	if(x<tilemap.size() && y<tilemap[0].size())
	{
//		std::cout<<"Tile blocked: "<<tilemap[x][y]<<std::endl;
		return tilemap[x][y];
	}
	return false;
}
