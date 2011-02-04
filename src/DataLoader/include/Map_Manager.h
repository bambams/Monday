#ifndef Map_Manager_h
#define Map_Manager_h


/** Loads the Map data from the "world#.cfg" config file.
 *
 * This contains:
 * - Tilemap:
 *   - [pixels wide] [pixels tall/high]
 *   - [path]
 * - Number of (expected) maps
 * - "invalid map" Map ID
 * - Base path ("world#/") where cnofig files can be found
 * - Maps:
 *   - Config file ("map#.cfg") for the map
 *   - Map ID (current map)
 *   - NORTH map ID
 *   - EAST map ID
 *   - SOUTH map ID
 *   - WEST map ID
 */

/* Defines Manager */
#include "Manager.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines */
#include "MapData.h"


#include <iostream>
#include <string>
#include <vector>


class Map_Manager
	: public Manager
{
	public:
		Map_Manager(const std::string &working_directory);
		virtual ~Map_Manager();

		std::string Get_WorkingDirectory();

		/* Once the Map Manager has been created, assign a single config
		 * file to load:
		 *   "world#.cfg"
		 *
		 * When the actual data will be pulled later, it will be a combination
		 * of the working directory and the individual config file:
		 *   workingDirectory + settings_config_file
		 */
		virtual void Set_ConfigFile(const std::string &config_file);
		virtual std::string Get_ConfigFile();


		/* The config file will contain a subdirectory ("world#/") where all the
		 * config file listed afterward will be contained within.
		 *
		 * This path is available AFTER calling Load_Settings_From_Config()
		 */
		virtual std::string Get_SubDirectory();


		/* Before calling Load_Settings_From_Config(), pass in the tile
		 * dimensions for the maps.
		 */
		void Set_Tile_Width(const u32 &width);
		u32 Get_Tile_Width();

		void Set_Tile_Height(const u32 &height);
		u32 Get_Tile_Height();


		/* Return the (loaded) mapArray to the World. */
		MapArray Get_MapArray();


		/* Virtual function: Actually extract the data from the config file */
		virtual bool Load_Settings_From_Config();

		/* Virtual function: Display all of data loaded into levelArray */
		virtual void Show_All(std::ostream &os);


		/* Read in the data from a single entry in the config file */

		/* Entry: "tilemap_config" */
		bool Read_Tilemap_Config(std::ifstream &fs);

		/* Entry: "num_maps" */
		bool Read_Num_Maps(std::ifstream &fs);

		/* Entry: "invalid_map_id" */
		bool Read_Invalid_Map_ID(std::ifstream &fs);

		/* Entry: "world_path" */
		bool Read_World_Path(std::ifstream &fs);

		/* Entry: "map_config" */
		bool Read_Map_Config(std::ifstream &fs);


	protected:

		/* This will never change for the life span of this manager, so it can
		 * be defined as a const.
		 *   "./media/world#/"
		 */
		const std::string workingDirectory;

		/* This can change, depending on which Map we want to load next:
		 *   "world#.cfg"
		 */
		std::string configFile;

		/* Manager::settings_config_file will contain this config file string */

		/* Path to the tileset that all Maps in this World will use. */
		std::string tileset_path;
		
		/* Contains the path to the config scripts (like "world#/") from the
		 * "world_path" line
		 */
		std::string subDirectory;
		
		/* ID of Map specifically designated to mean "no map found here" */
		map_id_type invalidID;
		
		/* You can get numMaps from mapArray.size() */
		MapArray mapArray;

		/* The data file itself will tell us that there are this many maps
		 * but we may actually read in more, or less, than this number.
		 * Mostly for santity check and debugging.
		 */
		u32 numMapsSpecified;

		/* Tiles probably won't ever be > 256 pixels (char), but u32 lets us read in
		 * from text files without needing to type-cast.
		 */
		u32 tileWidth;
		u32 tileHeight;
};


#endif  //  Map_Manager_h
