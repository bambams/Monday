#include "Manager.h"

/* Defines show_verbose() */
#include "Debug.h"

#include <iostream>
#include <string>


/**
 * Constructor: Manager, superclass to all *_Manager's
 */
Manager::Manager()
{
}


/**
 * Destructor: Manager
 */
Manager::~Manager()
{
}


/**
 * Function: Set the full path to the config file.  This superclass requires
 * the entire path, not just a relative path.
 */
void Manager::Set_ConfigFile(const std::string &file)
{
	configFile = file;
}


/**
 * Function: Return only the config file's full path.
 *
 * Returns: std::string Config file
 */
std::string Manager::Get_ConfigFile()
{
	return configFile;
}


/**
 * Function: Display all the settings saved in this manager (mostly just the
 * path where the Config File is pointed to).
 */
void Manager::Show_All(std::ostream &os)
{
	verbose_good(os, "Manager::Show_All()\n");
	verbose_good(os, "\tsettings_config_file: \"%s\"\n", configFile.c_str());
}
