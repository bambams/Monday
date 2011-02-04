#include "Obstacle_map.h"
#include <iostream>

Obstacle_map::Obstacle_map()
{
}


Obstacle_map::~Obstacle_map()
{
}


bool Obstacle_map::Load(std::ifstream &data)
{
	int width;
	int height;

	/* Get map width, handle unexpected EOF and bad data */
	data >> width;
	if (data.bad() || data.eof())
	{
		std::cerr << "Error: Map width could not be extracted. Obstacle_map.Load() failed.\n";
		return false;
	}

	/* Get map height, handle unexpected EOF and bad data */
	data >> height;
	if (data.bad() || data.eof())
	{
		std::cerr << "Error: Map height could not be extracted. Obstacle_map.Load() failed.\n";
		return false;
	}

	/* Get tile width, handle unexpected EOF and bad data */
	data >> tilesize_x;
	if (data.bad() || data.eof())
	{
		std::cerr << "Error: Tile width could not be extracted. Obstacle_map.Load() failed.\n";
		return false;
	}

	/* Get tile height, handle unexpected EOF and bad data */
	data >> tilesize_y;
	if (data.bad() || data.eof())
	{
		std::cerr << "Error: Tile height could not be extracted. Obstacle_map.Load() failed.\n";
		return false;
	}

	tilemap.resize(width);
	for (int y = 0; y < height; ++y)
	{
		for (Tilemap::iterator c = tilemap.begin(); c < tilemap.end(); ++c)
		{
			int obstacle;
			data >> obstacle;
			if (!data.bad() && !data.eof())
			{
				if (obstacle >= Obstacle::BLOCK_NONE && obstacle <= Obstacle::BLOCK_ALL)
				{
					c->push_back(obstacle);
				}
			}
			else
			{
				std::cerr << "Error: Map data on row " << y << " (0-based) could not be extracted. Obstacle_map.Load() failed.\n";
				return false;
			}
		}
	}

	/* Everything loaded fine */
	return true;
}


///************************************
/// Method:     Line_collision
/// FullName:   Obstacle_map::Line_collision
/// Access:     public
/// Returns:    false if no collision
///             true if collision occurred
/// Parameters: Vector &from - starting (x,y) coordinate
///             Vecotr &to   - ending (x,y) coordinate
///             Collision_data &data -
/// Task:       Tests whether the line between "from" and "to" intersects the
///             target object.
///************************************
bool Obstacle_map::Line_collision(const Vector &from, const Vector &to, Collision_data &data)
{
	//Todo: Fill in collision data

	Vector diff = to - from;

	// Pull expensive .X() and .Y() function calls into local variables
	float diffX = diff.X(),
	      diffY = diff.Y();
	float fromX = from.X(),
	      fromY = from.Y();
	float toX = to.X(),
	      toY = to.Y();

	float xslope = 0.0;
	if (diffY != 0)
	{
		xslope = diffX / diffY;
	}

	float yslope = 0.0;
	if (diffX != 0)
	{
		yslope = diffY / diffX;
	}

	float block = -1;

	float x = fromX;
	int x2 = (int)fromX;
	
	/* Object on left, wants to move to right */
	if (diffX > 0)
	{
		/* Why is "x" a float instead of an int? */
		x2 = (x2 / tilesize_x) * tilesize_x + tilesize_x;
		for (x = int(x / tilesize_x) * tilesize_x + tilesize_x; x < toX; x += tilesize_x)
		{
			float y = fromY + yslope * (x - fromX);
			if (Point_blocked(Vector(x, y), from))
			{
				block = (x - fromX) / diffX;
				break;
			}
			x2 += tilesize_x;
		}
	}
	if (diffX < 0)
	{
		for (x = int(x / tilesize_x) * tilesize_x - 1; x > toX; x -= tilesize_x)
		{
			float y = fromY + yslope * (x - fromX);
			if (Point_blocked(Vector(x, y), from))
			{
				block = (x - fromX) / diffX;
				break;
			}
		}
	}
	float y = fromY;
	if (diffY > 0)
	{
		for (y = int(y / tilesize_y) * tilesize_y + tilesize_y; y < toY; y += tilesize_y)
		{
			float x = fromX + xslope * (y - fromY);
			if (Point_blocked(Vector(x, y), from))
			{
				block = (y - fromY) / diffY;
				break;
			}
		}
	}
	if (diffY < 0)
	{
		for (y = int(y / tilesize_y) * tilesize_y - 1; y > toY; y -= tilesize_y)
		{
			float x = fromX + xslope * (y - fromY);
			if (Point_blocked(Vector(x, y), from))
			{
				block = (y - fromY) / diffY;
				break;
			}
		}
	}
/*	For every border x from x1 to x2
		y += yslope * (x - x1);
		control blocked

	For every border y from y1 to y2
		x += xslope * (y - y1);
		control blocked
*/
	if (block > 0)
		return false;
	return true;
}


bool Obstacle_map::Circle_collision(const Vector &center, float radius, Collision_data &data)
{
	//Todo: implement
	/*
	 * From tile center is on, check if circle goes into neighbouring tiles.
	 * Diagonally, distance to corners, gives adjustment towards center?
	 * Along axis, adjustment along axis.
	*/

	//Todo: fill in collision data

	data.obstacle = this;

	int tile_x = center.X() / tilesize_x;
	int tile_y = center.Y() / tilesize_y;

	float ontile_x = center.X() - tile_x * tilesize_x;
	float ontile_y = center.Y() - tile_y * tilesize_y;

	bool ret = false;

	if (ontile_x - radius < 0 && Tile_blocked(tile_x - 1, tile_y, tile_x, tile_y))
	{
		data.adjust += Vector(radius - ontile_x, 0);
		ret = true;
	}

	if (ontile_x + radius > tilesize_x && Tile_blocked(tile_x + 1, tile_y, tile_x, tile_y))
	{
		data.adjust += Vector(tilesize_x - (ontile_x + radius), 0);
		ret = true;
	}

	if (ontile_y - radius < 0 && Tile_blocked(tile_x, tile_y - 1, tile_x, tile_y))
	{
		data.adjust += Vector(0, radius - ontile_y);
		ret = true;
	}

	if (ontile_y + radius > tilesize_y && Tile_blocked(tile_x, tile_y + 1, tile_x, tile_y))
	{
		data.adjust += Vector(0, tilesize_y - (ontile_y + radius));
		ret = true;
	}

	if (ret)
	{
		return true;
	}

	float length;
	length = Vector(ontile_x, ontile_y).Length();
	if (length < radius && Tile_blocked(tile_x - 1, tile_y - 1, tile_x, tile_y))
	{
		float percent = (radius - length) / radius;
		data.adjust.Set(ontile_x * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tilesize_x, ontile_y).Length();
	if (length < radius  && Tile_blocked(tile_x + 1, tile_y - 1, tile_x, tile_y))
	{
		float percent = (radius - length) / radius;
		data.adjust.Set(-(tilesize_x - ontile_x) * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tilesize_x, ontile_y - tilesize_y).Length();
	if (length < radius && Tile_blocked(tile_x + 1, tile_y + 1, tile_x, tile_y))
	{
		float percent = (radius - length) / radius;
		data.adjust.Set(-(tilesize_x - ontile_x) * percent, -(tilesize_y - ontile_y) * percent);
		return true;
	}

	length = Vector(ontile_x, ontile_y - tilesize_y).Length();
	if (length < radius && Tile_blocked(tile_x - 1, tile_y + 1, tile_x, tile_y))
	{
		float percent = (radius - length) / radius;
		data.adjust.Set(ontile_x * percent, -(tilesize_y - ontile_y) * percent);
		return true;
	}

	return false;
}


bool Obstacle_map::Point_blocked(const Vector &position, const Vector &from)
{
	unsigned int x = position.X() / tilesize_x;
	unsigned int y = position.Y() / tilesize_y;
	return Tile_blocked(x, y, from.X(), from.Y());
}


bool Obstacle_map::Tile_blocked(unsigned int toX, unsigned int toY, unsigned int fromX, unsigned int fromY)
{
	if (toX < tilemap.size() && toY < tilemap[0].size())
	{
		int target = tilemap[toX][toY];

		/* Tile not blocked at all (at least by a map obstacle) */
		if (target == Obstacle::BLOCK_NONE)
		{
			return false;
		}

		/* Guaranteed block, regardless of direction trying to enter */
		if (target == Obstacle::BLOCK_ALL)
		{
			return true;
		}

		/* Wants to move onto this tile from the north? */
		if (fromY < toY && (target & Obstacle::NORTH) != 0)
		{
			return true;
		}

		/* Wants to move onto this tile from the west? */
		if (fromX < toX && (target & Obstacle::WEST) != 0)
		{
			return true;
		}

		/* Wants to move onto this tile from the east? */
		if (fromX > toX && (target & Obstacle::EAST) != 0)
		{
			return true;
		}

		/* Wants to move onto this tile from the south? */
		if (fromY > toY && (target & Obstacle::SOUTH) != 0)
		{
			return true;
		}

//		std::cout << "Tile blocked: " << tilemap[x][y] << std::endl;
//		return tilemap[x][y];
	}

	/* We should never get here.
	 *
	 * The only way we'll ever get here is if the obstacle map contains a
	 * value >= 16 (and the bottom 4 bits are "0000").  This is an undefined
	 * value, but since it IS "0000", it means NOT BLOCKED.
	 */
	return false;
}
