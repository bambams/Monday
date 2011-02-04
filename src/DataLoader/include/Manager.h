#ifndef Manager_h
#define Manager_h

/** Managers handle loading the configuration script files.
 *
 * Managers that are derived from this superclass must overload the virtual
 * functions.
 *
 * Each manager parses its own config file, which usually consists of paths
 * where child data can be found, or the actual data itself.  For example,
 * the Level's config file specifies where to find Layer and Trigger data,
 * while the Layer (or Trigger) config files actually contain the individual
 * tile and trigger values.
 */

#include <iostream>
#include <string>


class Manager
{
	public:
		Manager();
		virtual ~Manager();

		virtual void Set_ConfigFile(const std::string &file);
		std::string Get_ConfigFile();

		/* Virtual function: Load data from config file */
		virtual bool Load_Settings_From_Config() = 0;

		/* Virtual function: Display all the settings saved by the manager */
		virtual void Show_All(std::ostream &os);

	protected:
		/* Contains the entire path to a given config file:
		 *   "./global.cfg", or
		 *   "./media/world#.cfg", or
		 *   "./media/world#/map#.cfg", or
		 *   "./media/world#/map#/level#.cfg"
		 */
		std::string configFile;
};


#endif  //  Manager_h
