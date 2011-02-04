#include "Layer.h"

Layer::Layer()
:tileset(NULL)
{
}

void Layer::Load(std::ifstream& data, Resource_manager& resource_manager)
{
	//Todo: Make sure loading works, on all platforms.
	std::string tileset_file;
	std::getline(data, tileset_file);
	//Todo: Tileset should be loaded with resource manager
	tileset = new Tileset;
	tileset->Load(tileset_file, resource_manager);

	int width;
	int height;
	data>>width;
	data>>height;

	tilemap.resize(width);
	for (int y = 0; y < height; ++y)
	{
		for (Tilemap::iterator c = tilemap.begin(); c < tilemap.end(); ++c)
		{
			int index;
			data>>index;
			c->push_back(Tile(index));
		}
	}
}

void Layer::Render(const Camera& camera)
{
	int width = tileset->Tile_width();	
	int height = tileset->Tile_width();	

	Vector cam_apply = camera.Apply(Vector(0, 0));
	float cam_apply_x = cam_apply.X();
	float cam_apply_y = cam_apply.Y();
	for (int x = 0; x < static_cast<int>(tilemap.size()); ++x)
	{
		for (int y = 0; y < static_cast<int>(tilemap[x].size()); ++y)
		{
			tileset->Render(tilemap[x][y], x*width+cam_apply_x, y*height+cam_apply_y);
		}
	}
}
