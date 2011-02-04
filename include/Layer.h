#ifndef Layer_h
#define Layer_h

#include "Tileset.h"
#include "Camera.h"

#include <vector>
#include <fstream>

typedef std::vector<Tile> Column;
typedef std::vector<Column> Tilemap;


class Layer
{
	public:
		Layer();
		void Load(std::ifstream &data, Resource_manager *resource_manager);
		void Render(const Camera *camera);
	protected:
		Tileset *tileset;
		Tilemap tilemap;
};


#endif  //  Layer_h
