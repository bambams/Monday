#include "Map.h"
#include <fstream>
#include <iostream>

Map::Map()
:obstacle_manager(NULL)
{
}

Map::~Map()
{
	//TODO: This line causes crash... Figure out solution
//	obstacle_manager->Remove_obstacle(&obstacle_map);
}

void Map::Load(const std::string& filename, Obstacle_manager* iobstacle_manager, Resource_manager& resource_manager)
{
	obstacle_manager = iobstacle_manager;
	//Todo: Make sure loading works, on all platforms.
	std::ifstream fs;
	fs.open(filename.c_str());
	while(!fs.eof())
	{
		std::string line;
		std::getline(fs, line);
		if(line == "layer")
		{
			Layer layer;
			layer.Load(fs, resource_manager);
			layers.push_back(layer);
		}
		if(line == "obstaclemap")
		{
			std::cout<<"Loading obstacle map"<<std::endl;
			obstacle_map.Load(fs);
			obstacle_manager->Add_obstacle(&obstacle_map);
		}
	}
	fs.close();
}

void Map::Render(const Camera& camera)
{
	for(Layers::iterator i = layers.begin(); i!=layers.end(); ++i)
	{
		i->Render(camera);
	}
}

int Map::Get_Height()
{
	return height;
}

int Map::Get_Width()
{
	return width;
}
