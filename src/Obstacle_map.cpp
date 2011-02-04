#include "Obstacle_map.h"

#include "Debug.h"
#include "Parse_Lines.h"
#include "StdTypes.h"

#include <iostream>
#include <sstream>
#include <string>


Obstacle_map::Obstacle_map()
{
}


void Obstacle_map::Load(std::ifstream& data)
{
	//Todo: Make sure loading works, on all platforms.
	std::string tileset_file;

	Attributes attributes;

	std::string line;
	while (seek_non_comment_line(data, line))
	{
		if ("</obstaclemap>" == line)
		{
			/* Used to only need to type attribute name once: prevents typos */
			std::string attributeName;

			if (attributes.end() != attributes.find("width") && attributes.end() != attributes.find("height"))
			{
				Show_attributes(std::cout, attributes);

				/* Read in [row x column] points if both height and width are
				 * specified.
				 *
				 * If one or the other is not defined, do nothing.
				 */
				attributeName = "width";
				int width = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				attributeName = "height";
				int height = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				tilemap.resize(width);
				for (int y = 0; y < height; ++y)
				{
					/* Convert the current row into a number.  Then find the
					 * "0 = ..." pair using that number/index.
					 */
					std::stringstream row;
					row << y;
					if (attributes.end() != attributes.find(row.str()))
					{
						// Pull in the entire string value to a stream
						std::stringstream col(attributes[row.str()]);
						for (Tilemap::iterator c = tilemap.begin(); c < tilemap.end(); ++c)
						{
							int index;
							col >> index;
							c->push_back(1 == index);
						}
					}
				}
			}

			// Once the closing tag is found, do not scan the file any further.
			break;
		}

		//Store name, value pair in attributes
		std::string name;
		std::vector<std::string> value;

		/* Ensure that the line has a valid "name=value" pair, else we will
		 * see problems.
		 */
		if (true == name_and_value(line, name, value))
		{
			/* Until we figure out how to have multiple values with the same
			 * name, we're only going to be able to take the first one...
			 */
			attributes[name] = value[0];
		}
	}

//	int width;
//	int height;
//	data>>width;
//	data>>height;
//
//	data>>tilesize_x;
//	data>>tilesize_y;
//
//	tilemap.resize(width);
//	for (int y = 0; y < height; ++y)
//	{
//		for (Tilemap::iterator c = tilemap.begin(); c < tilemap.end(); ++c)
//		{
//			char index;
//			data>>index;
//			c->push_back(index == '1');
//		}
//	}
}


bool Obstacle_map::Line_collision(const Vector& from, const Vector& to, Collision_data& data)
{
	//Todo: Fill in collision data

	Vector diff = to - from;
	float xslope = diff.X()/diff.Y();
	float yslope = diff.Y()/diff.X();

	float block=-1;

	float x = from.X();
	if (diff.X() > 0)
	{
		for (x = int(x / tile_w) * tile_w + tile_w; x < to.X(); x += tile_w)
		{
			float y = from.Y() + yslope * (x-from.X());
			if (Point_blocked(Vector(x, y)))
			{
				block = (x-from.X())/diff.X();
				break;
			}
		}
	}
	if (diff.X() < 0)
	{
		for (x = int(x / tile_w) * tile_w - 1; x > to.X(); x -= tile_w)
		{
			float y = from.Y() + yslope * (x-from.X());
			if (Point_blocked(Vector(x, y)))
			{
				block = (x-from.X())/diff.X();
				break;
			}
		}
	}
	float y = from.Y();
	if (diff.Y() > 0)
	{
		for (y = int(y / tile_h) * tile_h + tile_h; y < to.Y(); y += tile_h)
		{
			float x = from.X() + xslope * (y-from.Y());
			if (Point_blocked(Vector(x, y)))
			{
				block = (y-from.Y())/diff.Y();
				break;
			}
		}
	}
	if (diff.Y() < 0)
	{
		for (y = int(y / tile_h) * tile_h - 1; y > to.Y(); y -= tile_h)
		{
			float x = from.X() + xslope * (y-from.Y());
			if (Point_blocked(Vector(x, y)))
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
	if (block > 0)
	{
		return false;
	}

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

	int tile_x = center.X() / tile_w;
	int tile_y = center.Y() / tile_h;

	float ontile_x = center.X() - tile_x * tile_w;
	float ontile_y = center.Y() - tile_y * tile_h;

	bool ret = false;

	if (ontile_x-radius < 0 && Tile_blocked(tile_x-1, tile_y))
	{
		data.adjust+=Vector(radius-ontile_x, 0);
		ret = true;
	}

	if (ontile_x+radius > tile_w && Tile_blocked(tile_x+1, tile_y))
	{
		data.adjust+=Vector(tile_w - (ontile_x+radius), 0);
		ret = true;
	}

	if (ontile_y-radius < 0 && Tile_blocked(tile_x, tile_y-1))
	{
		data.adjust+=Vector(0, radius-ontile_y);
		ret = true;
	}

	if (ontile_y+radius > tile_h && Tile_blocked(tile_x, tile_y+1))
	{
		data.adjust+=Vector(0, tile_h - (ontile_y+radius));
		ret = true;
	}

	if (ret)
	{
		return true;
	}

	float length;
	length = Vector(ontile_x, ontile_y).Length();
	if (length < radius && Tile_blocked(tile_x-1, tile_y-1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(ontile_x * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tile_w, ontile_y).Length();
	if (length < radius  && Tile_blocked(tile_x+1, tile_y-1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(-(tile_w-ontile_x) * percent, ontile_y * percent);
		return true;
	}

	length = Vector(ontile_x - tile_w, ontile_y - tile_h).Length();
	if (length < radius && Tile_blocked(tile_x+1, tile_y+1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(-(tile_w-ontile_x) * percent, -(tile_h-ontile_y) * percent);
		return true;
	}

	length = Vector(ontile_x, ontile_y - tile_h).Length();
	if (length < radius && Tile_blocked(tile_x-1, tile_y+1))
	{
		float percent = (radius-length)/radius;
		data.adjust.Set(ontile_x * percent, -(tile_h-ontile_y) * percent);
		return true;
	}

	return false;
}


bool Obstacle_map::Point_blocked(const Vector& position)
{
	unsigned int x = position.X()/tile_w;
	unsigned int y = position.Y()/tile_h;
	return Tile_blocked(x, y);
}


bool Obstacle_map::Tile_blocked(unsigned int x, unsigned int y)
{
	if (x < tilemap.size() && y < tilemap[0].size())
	{
//		std::cout<<"Tile blocked: "<<tilemap[x][y]<<std::endl;
		return tilemap[x][y];
	}
	return false;
}


void Obstacle_map::Set_Dimensions(const int& tilew, const int& tileh)
{
	tile_w = tilew;
	tile_h = tileh;
}
