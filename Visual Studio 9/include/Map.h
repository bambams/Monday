#ifndef Map_h
#define Map_h


#include "Camera.h"
#include "Container.h"
#include "Entity_manager.h"
#include "Layer.h"
#include "Main.h"
#include "Obstacle_manager.h"
#include "Obstacle_map.h"
#include "Tileset.h"

#include <string>
#include <vector>


typedef std::vector<Layer> Layers;


/* Class: Map
 */
class Map: public Container
{
	public:

		/* Constructor: Map
		 */
		Map();

		/* Destructor: Map
		 */
		~Map();

		/* Function: Load
		 * Loads a map from file.
		 *
		 * Parameters:
		 * filename - file to load
		 * obstacle_manager - for registering obstacles on the map
		 * resource_manager - for resource loading
		 * entity_manager - for loading entities
		 */
		void Load(const std::string& filename, Obstacle_manager* obstacle_manager, Resource_manager& resource_manager, Entity_manager* entity_manager);

		/* Function: Load_Dimensions
		 * Loads the dimension of tiles that all sub-components use.
		 */
		void Load_Dimensions(std::ifstream& data);

		/* Function: Get_TileWidth
		 * Returns width of all tiles and obstacles found in this map.
		 */
		int Get_TileWidth()  { return tile_w; }

		/* Function: Get_TileHeight
		 * Returns height of all tiles and obstacles found in this map.
		 */
		int Get_TileHeight() { return tile_h; }

		/* Function: Add_entity
		 * Overloaded from <Container> for extra handling.
		 */
		virtual void Add_entity(Entity* e);

		/* Function: Remove_entity
		 * Overloaded from <Container> for extra handling.
		 */
		virtual void Remove_entity(Entity* e);

		/* Function: Render
		 * Render everything on the map
		 */
		void Render(const Camera& camera);

//		int Get_Height();
//		int Get_Width();

	protected:
		int tile_w, tile_h;
//		int width, height;

		Layers layers;
		Obstacle_manager* obstacle_manager;
		Obstacle_map obstacle_map;
};


#endif  //  Map_h
