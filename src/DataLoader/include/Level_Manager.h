#ifndef Level_Manager_h
#define Level_Manager_h


/** Loads the Level data from the "map#.cfg" config file.
 *
 * This contains:
 * - Map dimensions
 * - Number of (expected) levels
 * - Base directory where Level and Trigger config files will be found
 * - Levels:
 *   - ID
 *   - path to a Level config file
 *   - path to a Trigger config file
 *   - Parallax data
 */

/* Defines Manager */
#include "Manager.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

/* Defines parallax_type, LevelData, LevelArray */
#include "LevelData.h"


#include <iostream>
#include <string>
#include <vector>


class Level_Manager
	: public Manager
{
	public:
		Level_Manager(const std::string &working_directory);
		virtual ~Level_Manager();

		std::string Get_WorkingDirectory();

		/* Once the Level Manager has been created, assign a single config
		 * file to load:
		 *   "map#.cfg"
		 *
		 * When the actual data will be pulled later, it will be a combination
		 * of the working directory and the individual config file:
		 *   workingDirectory + settings_config_file
		 */
		virtual void Set_ConfigFile(const std::string &config_file);
		virtual std::string Get_ConfigFile();


		/* The config file will contain a subdirectory ("map#/") where all the
		 * config file listed afterward will be contained within.
		 *
		 * This path is available AFTER calling Load_Settings_From_Config()
		 */
		virtual std::string Get_SubDirectory();


		/* After Load_Settings_From_Config() has parsed the config file, the
		 * dimensions for the Level can be found.
		 */
		u32 Get_Num_Rows();
		u32 Get_Num_Columns();


		/* Return the (loaded) levelArray to the Map. */
		LevelArray Get_LevelArray();


		/* Virtual function: Actually extract the data from the config file */
		virtual bool Load_Settings_From_Config();

		/* Virtual function: Display all of data loaded into layerArray */
		virtual void Show_All(std::ostream &os);


		/* Read in the data from a single entry in the config file */

		/* Entry: "map_dimensions" */
		bool Read_Map_Dimensions(std::ifstream &fs);

		/* Entry: "num_levels" */
		bool Read_Num_Levels(std::ifstream &fs);

		/* Entry: "map_path" */
		bool Read_Map_Path(std::ifstream &fs);

		/* Entry: "levels_config" */
		bool Read_Levels_Config(std::ifstream &fs);


	protected:

		/* This will never change for the life span of this manager, so it can
		 * be defined as a const.
		 *   "./media/world#/map#/"
		 */
		const std::string workingDirectory;

		/* This can change, depending on which Level we want to load next:
		 *   "map#.cfg"
		 */
		std::string configFile;

		/* Manager::settings_config_file will contain this config file string */

		/* "map#.cfg" has a directory name listed which specifies where all of
		 * the following config files will be found under.
		 */
		std::string subDirectory;

		/* You can get numLevels from levelArray.size() */
		LevelArray levelArray;

		/* The data file itself will tell us that there are this many levels
		 * but we may actually read in more, or less, than this number.
		 * Mostly for santity check and debugging.
		 */
		u32 numLevelsSpecified;

		u32 numRows;
		u32 numColumns;
};


#endif  //  Level_Manager_h
