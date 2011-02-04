#include "Debug.h"
#include "StdTypes.h"

#include <iostream>
#include <stdarg.h>
#include <stdlib.h>
#include <string>
#include <vector>


/* Debugging messages (default: off) */
u32 showDebug = 0;


/** Function: debugOn
 *
 * Turn on debugging information at a specific level.
 *
 * Parameter: os Stream to report which debugging level has been turned on.
 */
void debugOn(std::ostream& os, const int& level)
{
	/* Setting the debug level to 0 is the same as turning it off. */
	if (0 == level)
	{
		debugOff(os);
		return;
	}

	/* These are defined in "Debug.h", and are used here to show the user what
	 * level of error details will be displayed.
	 */
	std::vector<std::string> warningLevels;
	warningLevels.push_back("CRITICAL_LEVEL");
	warningLevels.push_back("ERROR_LEVEL");
	warningLevels.push_back("WARNING_LEVEL");
	warningLevels.push_back("SUGGESTION_LEVEL");
	warningLevels.push_back("VERBOSE_LEVEL4");
	warningLevels.push_back("VERBOSE_LEVEL3");
	warningLevels.push_back("VERBOSE_LEVEL2");
	warningLevels.push_back("VERBOSE_LEVEL1");

	showDebug = level;

	if (showDebug >= warningLevels.size())
		showDebug = warningLevels.size() - 1;

	/* This is to prevent an out-of-range error if someone updates the enum
	 * in Debug.h without also updating this list.
	 */
	Monday_out(showDebug, os, "Setting debug ON to level %d (%s).\n",
	           showDebug, warningLevels[showDebug].c_str());

	/* And this warns the stupid person of the problem mentioned above. */
	if (warningLevels.size() < NUM_DEBUG_DEFINITIONS)
	{
		std::cerr << "*CRITICAL ERROR: The LevelDefinitions enum in Debug.h has "
		          << NUM_DEBUG_DEFINITIONS << " definitions, while the warningLevels vector in debugOn() only has "
		          << warningLevels.size() << " defined.  Update debugOn().\n";
		return;
	}
}


/** Function: debugOff
 *
 * Turn off debugging information.
 *
 * Parameter: os Stream to report that debugging has been turned off.
 */
void debugOff(std::ostream& os)
{
	showDebug = 0;
	Monday_out(showDebug, os, "Turning debug OFF.\n");
}


/**
 * Function: If condition does not evaluate true, dump file and line where
 * condition failed, and exit program.
 */
void Monday_assert(const char* e, const char* file, int line)
{
	std::cout << "assert() failed at " << file << ":" << line << ":\n\t" << e << std::endl;
	exit(0);
}


void Monday_log(const std::string& e, const char* file, int line)
{
	std::cout << file << ":" << line << ":" << " Log: " << e << std::endl;
}


/**
 * Function: Monday_out
 *
 * Pass formatted output to the given stream.  Output is only shown when
 * debugLevel <= showDebug, which level is specified by the user at runtime
 * from the command prompt.
 *
 * Parameter: debugLevel Level of "importance" of the message.  Levels closer
 *            to 1 are more critical, while higher values indicate "not as
 *            important" messages.  (Note that a value of 0 will display even
 *            when debugging is turned off entirely.)
 *
 * Parameter: os Output stream (like std::cout or std::cerr).
 *
 * Parameter: msg Formatted string to display.  It takes the same parameters
 *            as (s)printf.  Must be 1024 characters or less in length
 *            (including all formatted text).
 */
void Monday_out(const u32 &debugLevel, std::ostream& os, const char* msg, ...)
{
	/* Messages are only displayed when debugLevel <= showDebug */
	if (debugLevel <= showDebug)
	{
		char buf[1024];

		va_list ap;
		va_start(ap, msg);
		vsprintf(buf, msg, ap);
		va_end(ap);

		os << buf;
	}
}


/**
 * Function: Show_attributes
 *
 * Display both the key and value for an Attributes map.
 *
 * Since this is a non-critical message, it is only shown when the debug value
 * is set high enough.
 *
 * Parameter: attributes Defined as std::map<std::string, std::string> pair.
 *            Most commonly found in data as a "name = value" pair.
 */
void Show_attributes(std::ostream& os, Attributes& attributes)
{
	Monday_out(VERBOSE_LEVEL2, os, "- Num attributes: %d\n", attributes.size());
	for(Attributes::iterator i = attributes.begin(); i != attributes.end(); ++i)
	{
		Monday_out(VERBOSE_LEVEL2, os, "  \"%s\" = \"%s\"\n", i->first.c_str(), i->second.c_str());
	}
}
