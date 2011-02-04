/**
 * Load all of the Triggers for only the CURRENT Layer in from a file
 *
 *  The Trigger Manager handles all of the actual disk IO, leaving "Layer" to
 *  simply be a regular class object.
 *
 */

#include "Trigger_Manager.h"

/* Defines verbose_bad, verbose_good */
#include "Debug.h"

/* Defines fileExists function */
#include "DiskAccess.h"

/* Defines trigger_type, TriggerData, TriggerArray
 */
#include "TriggerData.h"

/* Defines LayerArray */
#include "Level.h"

/* Magic function that scans a config file for the first non-linecomment */
#include "Parse_Lines.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"


#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <stdlib.h>


/**
 * Constructor: Trigger_Manager, passes base directory to the Manager
 * superclass
 */
Trigger_Manager::Trigger_Manager(const std::string &working_directory)
	: workingDirectory(working_directory)
{
	numTriggersSpecified = 0;

	numRows = 0;
	numColumns = 0;
}


/**
 * Destructor: Trigger_Manager
 */
Trigger_Manager::~Trigger_Manager()
{
	// Nothing to do
}


/**
 * Function: The working directory is considered the "root" directory for all
 * derived subdirectories.
 *
 * Returns: std::string Working directory
 */
std::string Trigger_Manager::Get_WorkingDirectory()
{
	return workingDirectory;
}


/**
 * Function: Set the path to the config file.  The Manager superclass receives
 * the FULL config file path, which includes the working directory as well.
 */
void Trigger_Manager::Set_ConfigFile(const std::string &config_file)
{
	configFile = config_file;
	Manager::Set_ConfigFile(workingDirectory + config_file);
}


/**
 * Function: Return only the config file's path as was given to this object:
 * not including the working directory.
 *
 * Returns: std::string Config file
 */
std::string Trigger_Manager::Get_ConfigFile()
{
	return configFile;
}


/**
 * Function: Set the number of rows that are expected within the config file
 * ("layer#_triggers.cfg").
 */
void Trigger_Manager::Set_Num_Rows(const u32 &rows)
{
	numRows = rows;
}


/**
 * Function: Set the number of cols that are expected within the config file
 * ("layer#_triggers.cfg").
 */
void Trigger_Manager::Set_Num_Columns(const u32 &cols)
{
	numColumns = cols;
}


/**
 * Function: Return the Trigger array (contains all of the Trigger entries
 * found within the current Level specified in the config file).
 *
 * Returns: std::vector Trigger Array
 */
TriggerArray Trigger_Manager::Get_TriggerArray()
{
	return triggerArray;
}


/**
 * Function: Load levels from the "level#.cfg" config file
 *
 *  The "level#.cfg" file should have been saved to Manager before calling
 *  this function.
 *
 * Returns: false if any data could not be read in, true otherwise.
 */
bool Trigger_Manager::Load_Settings_From_Config()
{
	/* Assume success unless otherwise indicated */
	bool success = true;

	std::string fullFilePath = Manager::Get_ConfigFile();

	/* If the file doesn't exist, or if the number of rows/columns are not
	 * defined, return with an error.
	 */
	if (fileExists(fullFilePath) == false)
	{
		verbose_bad(std::cerr, "fileExists(\"%s\") returned false.\n", fullFilePath.c_str());
		success = false;
	}

	std::ifstream fs(fullFilePath.c_str());
	if (!fs.is_open())
	{
		verbose_bad(std::cerr, "Could not load file: \"%s\"", fullFilePath.c_str());
		success = false;
	}

	std::string line;
	while (success != false && seek_non_comment_line(fs, line))
	{
		if (line == "num_triggers")
		{
			success = Read_Num_Triggers(fs);
			continue;
		}

		else if (line == "triggers_config")
		{
			success = Read_Triggers_Config(fs);
			continue;
		}

		else
		{
			// Something was here that we weren't expecting.  Exit gracefully?  Or just return false?
			success = false;
		}
	}

	fs.close();
	return success;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "num_triggers"
 *
 * Format:
 * - [number of triggers]
 *
 * Returns: false if any data could not be read in, true otherwise.
 */
bool Trigger_Manager::Read_Num_Triggers(std::ifstream &fs)
{
	std::string line_num_triggers;

	if (seek_non_comment_line(fs, line_num_triggers))
	{
		std::stringstream ss(line_num_triggers);

		ss >> numTriggersSpecified;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: invalid value after \"num_triggers\"\n");
			return false;
		}
		else
		{
			verbose_good(std::cerr, "Level has specified %d trigger%s will be found within the \"%s\" config file.\n",
			             numTriggersSpecified, (numTriggersSpecified == 1 ? "" : "s"), Manager::Get_ConfigFile().c_str());
		}
	}
	else
	{
		verbose_bad(std::cerr, "num_triggers specified, but could not read in any more data.\n");
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Read in the entry from the config file that starts with
 *   "triggers_config"
 *
 * Format:
 * - [trigger ID]
 * - [x coord] [y coord]
 * - [trigger type]
 *
 * Returns: false if any data could not be read in, true otherwise (note that
 *          out-of-bounds coordinates do not result in a FALSE).
 */
bool Trigger_Manager::Read_Triggers_Config(std::ifstream &fs)
{
	std::string line_triggers_config;

	TriggerData triggerData;

	if (seek_non_comment_line(fs, line_triggers_config))
	{
		std::stringstream ss(line_triggers_config);

		ss >> triggerData.id;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in trigger's ID.\n");
			return false;
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: Could not read in trigger ID.\n");
		return false;
	}

	if (seek_non_comment_line(fs, line_triggers_config))
	{
		std::stringstream ss(line_triggers_config);

		ss >> triggerData.x;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in trigger's x-coordinate.\n");
			return false;
		}

		if (triggerData.x >= numColumns)
		{
			verbose_bad(std::cerr, "*WARNING: Trigger's x-coordinates are outside of the Level's horizontal dimensions.\n");
			/* Unlike most other read errors, if a trigger is outside of the
			 * Map/Level boundaries, it can be ignored.  It's possible that
			 * the placement was intentional by the designers.  However, if
			 * "verbose" is on, they will get a WARNING (not an ERROR) message
			 * telling us about it.
			 */
		}

		ss >> triggerData.y;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in trigger's y-coordinate.\n");
			return false;
		}

		if (triggerData.y >= numRows)
		{
			verbose_bad(std::cerr, "*WARNING: Trigger's y-coordinates are outside of the Level's vertical dimensions.\n");
			/* Unlike most other read errors, if a trigger is outside of the
			 * Map/Level boundaries, it can be ignored.  It's possible that
			 * the placement was intentional by the designers.  However, if
			 * "verbose" is on, they will get a WARNING (not an ERROR) message
			 * telling us about it.
			 */
		}
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: Could not read in trigger's coordinates line.\n");
		return false;
	}

	if (seek_non_comment_line(fs, line_triggers_config))
	{
		std::stringstream ss(line_triggers_config);

		std::string triggerTypes;
		ss >> triggerTypes;
		if (ss.fail())
		{
			verbose_bad(std::cerr, "*ERROR: Could not read in trigger's type.\n");
			return false;
		}

		if (triggerTypes == "TRIGGER_LANDING_SPOT")
		{
			triggerData.type = TRIGGER_LANDING_SPOT;
		}
		else if (triggerTypes == "TRIGGER_ACTION")
		{
			triggerData.type = TRIGGER_ACTION;
		}
		else
		{
			verbose_bad(std::cerr, "*ERROR: Unknown trigger type: \"%s\"\n", triggerTypes.c_str());
			return false;
		}
		triggerArray.push_back(triggerData);
	}
	else
	{
		verbose_bad(std::cerr, "*ERROR: Could not read in trigger type.\n");
		return false;
	}

	/* If we got here, the data read in correctly */
	return true;
}


/**
 * Function: Output all the settings saved in this manager (mostly paths and
 * stuff) to the specified stream.  This is mostly a debugging tool to show
 * that the data were collected correctly.
 *
 * Flags: - If "debug" is specified before calling this function, extra non-
 *          error debugging info is displayed.
 *        - If "verbose" is specified before calling this function, extra
 *          error debugging info is displayed (good for tracking down "what
 *          didn't load").
 */
void Trigger_Manager::Show_All(std::ostream &os)
{
	u32 triggerArraySize = triggerArray.size();
	if (triggerArraySize == 0)
	{
		/* No Trigger data were loaded; no reason to print anything here */
		return;
	}

	verbose_good(os, "Trigger_Manager:\n");

	/* Show the main manager's Show_All() function before our own. */
	Manager::Show_All(os);

	os << "\nTrigger_Manager::Show_All()\n";

	verbose_good(os, "\tDimensions for layers: %d rows, %d columns\n", numRows, numColumns);

	/* Display each of the triggers */

	verbose_good(os, "\n\tTriggers:\n");
	if (triggerArraySize != numTriggersSpecified)
	{
		/* Number specified, and number found were not equal.  Print debug
		 * info stating as such.
		 */
		verbose_good(os, "\t\tNumber of triggers specified in config file: %d\n", numTriggersSpecified);
		verbose_good(os, "\t\tActual number of triggers found: %d\n\n", triggerArraySize);
	}
	else
	{
		verbose_good(os, "\t\t%d trigger%s found.\n", triggerArraySize, (triggerArraySize == 1 ? "" : "s"));
	}

	/* Display the entire TriggerData structure */
	for (u32 triggerIndex = 0; triggerIndex < triggerArraySize; ++triggerIndex)
	{
		TriggerData triggerData = triggerArray[triggerIndex];

		/* Display the Trigger ID, x/y coordinates, and Trigger type */
		os << "\tTrigger " << triggerIndex << ":\n";
		os << "\t\tID: " << triggerData.id << std::endl;
		os << "\t\tCoordinates: " << triggerData.x << ", " << triggerData.y << std::endl;
		os << "\t\tType: "
		   << (triggerData.type == TRIGGER_LANDING_SPOT ? "TRIGGER_LANDING_SPOT\n" : "")
		   << (triggerData.type == TRIGGER_ACTION ? "TRIGGER_ACTION\n" : "");
		os << std::endl;
	}
}
