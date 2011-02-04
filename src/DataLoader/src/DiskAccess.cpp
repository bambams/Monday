#include "DiskAccess.h"

#include <string>
#include <fstream>


/**
 * Function: Check to see if a file exists.
 *
 * Input: std::string fileName File to check to make sure it exists
 *
 * Returns: false on failure (file could not be opened), else true on success
 */
bool fileExists(const std::string &fileName)
{
	bool success = true;

	std::ifstream fs(fileName.c_str());
	if (fs.fail())
	{
		success = false;
	}

	fs.close();

	return success;
}
