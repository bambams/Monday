#ifndef Layer_Manager_h
#define Layer_Manager_h


/** Loads in the Layer data from the "level#.cfg" config file.
 *
 * This config file doesn't contain paths to any other config files, so this
 * manager will load all of the data (obstacles, [row x col] tiles) directly.
 */

/* Defines Manager */
#include "Manager.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines tile_type, obstacle_type, ObstacleLine, ObstacleMap, LayerLine,
 * LayerMap, LayerData, LayerArray
 */
#include "LayerData.h"


#include <iostream>
#include <string>
#include <vector>


class Layer_Manager
	: public Manager
{
	public:
		/* When creating the Layer Manager, pass in the directory where all
		 * the config files will be found under:
		 *   "./media/world#/map#/"
		 */
		Layer_Manager(const std::string &working_directory);
		virtual ~Layer_Manager();

		std::string Get_WorkingDirectory();

		/* Once the Layer Manager has been created, assign a single config
		 * file to load:
		 *   "level#.cfg"
		 *
		 * When the actual data will be pulled later, it will be a combination
		 * of the working directory and the individual config file:
		 *   workingDirectory + settings_config_file
		 */
		virtual void Set_ConfigFile(const std::string &config_file);
		virtual std::string Get_ConfigFile();


		/* Before calling Load_Settings_From_Config(), pass in the row and col
		 * dimensions for the layers.
		 */
		void Set_Num_Rows(const u32 &rows);
		u32 Get_Num_Rows();

		void Set_Num_Columns(const u32 &cols);
		u32 Get_Num_Columns();


		/* Return the (loaded) layerArray to the Level. */
		LayerArray Get_LayerArray();


		/* Virtual function: Actually extract the data from the config file */
		virtual bool Load_Settings_From_Config();

		/* Virtual function: Display all of data loaded into layerArray */
		virtual void Show_All(std::ostream &os);


		/* Read in the data from a single entry in the config file */

		/* Entry: "obstacles_config" */
		bool Read_Obstacles_Config(std::ifstream &fs);

		/* Entry: "num_layers" */
		bool Read_Num_Layers(std::ifstream &fs);

		/* Entry: "layers_config" */
		bool Read_Layers_Config(std::ifstream &fs);


	protected:

		/* This will never change for the life span of this manager, so it can
		 * be defined as a const.
		 *   "./media/world#/map#/"
		 */
		const std::string workingDirectory;

		/* This can change, depending on which Layer we want to load next:
		 *   "level#.cfg"
		 */
		std::string configFile;

		/* Manager::settings_config_file will contain this config file string */

		/* You can get numLayers from layerArray.size() */
		LayerArray layerArray;

		ObstacleMap obstacles;

		/* The data file itself will tell us that there are this many layers
		 * but we may actually read in more, or less, than this number.
		 * Mostly for santity check and debugging.
		 */
		u32 numLayersSpecified;

		u32 numRows;
		u32 numColumns;
};


#endif  //  Layer_Manager_h
