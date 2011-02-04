#ifndef Layer_h
#define Layer_h

#include "Camera.h"
#include "Tileset.h"

#include <fstream>
#include <vector>

typedef std::vector<Tile> Column;
typedef std::vector<Column> Tilemap;


/* Class: Layer
 * Graphical tilemap
 */
class Layer
{
	public:
		/* Constructor: Layer
		 */
		Layer();

		/* Function: Set_Dimensions
		 * Sets the dimension of the tiles within the tilemap; must be called
		 * before Load().
		 */
		void Set_Dimensions(const int& tilew, const int& tileh);

		/* Function: Load
		 * Parameters:
		 * data - filestream to load from.
		 * resource_manager - needed for loading resources.
		 */
		void Load(std::ifstream& data, Resource_manager& resource_manager);

		/* Function: Render
		 * Renders the tilemap
		 */
		void Render(const Camera& camera);

	protected:
		int tile_w, tile_h;

		Tileset tileset;
		Tilemap tilemap;
};


#endif  //  Layer_h
