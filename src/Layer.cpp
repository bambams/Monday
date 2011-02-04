#include "Layer.h"

#include "Debug.h"
#include "Parse_Lines.h"
#include "StdTypes.h"
#include "Tileset.h"

#include <iostream>
#include <sstream>
#include <string>



Layer::Layer()
{
}


void Layer::Load(std::ifstream& data, Resource_manager& resource_manager)
{
	//Todo: Make sure loading works, on all platforms.
	Attributes attributes;

	std::string line;
	while (seek_non_comment_line(data, line))
	{
		if ("</layer>" == line)
		{
			/* Used to only need to type attribute name once: prevents typos */
			std::string attributeName;

			attributeName = "tilemap";
			if (attributes.end() != attributes.find(attributeName))
			{
				Show_attributes(std::cout, attributes);

				//Todo: Tileset should be loaded with resource manager
				Monday_out(VERBOSE_LEVEL2, std::cout, "Layer:Load()\n\tfile: \"%s\"\n", attributes[attributeName].c_str());
				tileset.Set_Dimensions(tile_w, tile_h);
				tileset.Load(attributes[attributeName], resource_manager);
				attributes.erase(attributeName);
			}

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
							c->push_back(Tile(index));
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
}


void Layer::Render(const Camera& camera)
{
	Vector cam_apply = camera.Apply(Vector(0, 0));
	float cam_apply_x = cam_apply.X();
	float cam_apply_y = cam_apply.Y();

	int cols = static_cast<int>(tilemap.size());
	for (int x = 0; x < cols; ++x)
	{
		int rows = static_cast<int>(tilemap[x].size());
		for (int y = 0; y < rows; ++y)
		{
			tileset.Render(tilemap[x][y], x * tile_w + cam_apply_x, y * tile_h + cam_apply_y);
		}
	}
}


void Layer::Set_Dimensions(const int& tilew, const int& tileh)
{
	tile_w = tilew;
	tile_h = tileh;
}
