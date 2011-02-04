#ifndef Map_h
#define Map_h

#include "main.h"
#include "Layer.h"
#include "Camera.h"
#include "Obstacle_map.h"
#include "Obstacle_manager.h"
#include "Obstacle_visualizer.h"


#include <string>
#include <vector>


typedef std::vector<Layer> Layers;

class Map
{
	public:
		Map();
		~Map();
		bool Load(const std::string &filename, Obstacle_manager *obstacle_manager, Resource_manager *resource_manager);

		/* Draw all the layers */
		void Render(const Camera *camera);

		int Get_Height();
		int Get_Width();

	protected:
		int tile_w, tile_h;
		int width, height;

		Layers layers;
		Obstacle_manager *obstacle_manager;
		Obstacle_map obstacle_map;
		Obstacle_visualizer obstacle_visualizer;
};

#endif  //  Map_h
