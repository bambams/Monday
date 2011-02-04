#include "Map.h"

#include "Debug.h"
#include "Entity.h"
#include "Layer.h"
#include "Obstacle_map.h"
#include "Parse_Lines.h"

#include <fstream>
#include <iostream>


Map::Map()
:obstacle_manager(NULL)
{
	/* Assume that all tiles are 32x32, but can be overridden */
	tile_w = 32;
	tile_h = 32;
}


Map::~Map()
{
	obstacle_manager->Remove_obstacle(&obstacle_map);
	Entities entities = Container::Get_entities();
	for (Entities::iterator i = entities.begin(); i!=entities.end(); ++i)
	{
		if ((*i)->Is_solid())
		{
			obstacle_manager->Remove_obstacle((*i));
		}
	}
}


void Map::Load_Dimensions(std::ifstream& data)
{
	Attributes attributes;

	std::string line;
	while (seek_non_comment_line(data, line))
	{
		if ("</map>" == line)
		{
			/* Used to only need to type attribute name once: prevents typos */
			std::string attributeName;

			if (attributes.end() != attributes.find("tile_width") &&
			    attributes.end() != attributes.find("tile_height"))
			{
				Show_attributes(std::cout, attributes);

				attributeName = "tile_width";
				tile_w = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);

				attributeName = "tile_height";
				tile_h = String_to_type<int>(attributes[attributeName]);
				attributes.erase(attributeName);
			}
			else
			{
				Monday_out(WARNING_LEVEL, std::cout, "*WARNING: Didn't find both width and height.\n");
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
		else
		{
			/* If this fails, the line doesn't have a "name=value" pair.
			 * It's not an error we can't recover from, so this is where a
			 * warning can be displayed, if desired.
			 */
			Monday_out(VERBOSE_LEVEL4, std::cout, "No attribute pair found: %s, %s", line.c_str(), name.c_str());
		}
	}
}


void Map::Load(const std::string& filename, Obstacle_manager* iobstacle_manager,
			   Resource_manager& resource_manager, Entity_manager* entity_manager)
{
	obstacle_manager = iobstacle_manager;
	//Todo: Make binary loading
	std::ifstream fs;
	fs.open(filename.c_str());
	mon_assert(fs.is_open() && "Could not open specified filename");
	Monday_out(VERBOSE_LEVEL3, std::cout, "Map:Load()\n\tFilename: \"%s\"\n", filename.c_str());
	while (!fs.eof())
	{
		std::string line;
		std::getline(fs, line);
		if ("<map>" == line)
		{
			Monday_out(VERBOSE_LEVEL2, std::cout, "Loading tile dimensions\n");
			Load_Dimensions(fs);
		}

		else if ("<layer>" == line)
		{
			Monday_out(VERBOSE_LEVEL2, std::cout, "Loading map layer\n");
			Layer layer;
			layer.Set_Dimensions(tile_w, tile_h);
			layer.Load(fs, resource_manager);
			layers.push_back(layer);
		}

		else if ("<obstaclemap>" == line)
		{
			Monday_out(VERBOSE_LEVEL2, std::cout, "Loading obstacle map\n");
			obstacle_map.Set_Dimensions(tile_w, tile_h);
			obstacle_map.Load(fs);
			obstacle_manager->Add_obstacle(&obstacle_map);
		}

		else if ("<entity>" == line)
		{
			Monday_out(VERBOSE_LEVEL2, std::cout, "Loading entity data\n");
			Entity* entity = entity_manager->Create_entity(this);

			/* This must be set before Load() is called */
			entity->Set_Dimensions(tile_w, tile_h);
			entity->Load(fs);
		}
	}
	fs.close();
}


void Map::Render(const Camera& camera)
{
	for (Layers::iterator i = layers.begin(); i!=layers.end(); ++i)
	{
		i->Render(camera);
	}
       	Container::Render(camera);
}


/*
int Map::Get_Height()
{
	return height;
}


int Map::Get_Width()
{
	return width;
}
*/

void Map::Add_entity(Entity* entity)
{
	Container::Add_entity(entity);
	if (entity->Is_solid())
	{
		obstacle_manager->Add_obstacle(entity);
	}
}


void Map::Remove_entity(Entity* entity)
{
	Container::Remove_entity(entity);
	if (entity->Is_solid())
	{
		obstacle_manager->Remove_obstacle(entity);
	}
}
