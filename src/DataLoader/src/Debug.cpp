#include "Debug.h"

#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <stdarg.h>


/**
 * Function: If condition does not evaluate true, dump file and line where
 * condition failed, and exit program.
 */
void Monday_assert(const char *e, const char *file, int line)
{
	std::cout << "assert(" << e << ") failed at " << file << ":" << line << std::endl;
	exit(0);
}


/**
 * Function: Show output to std::cerr whenever enableVerbose is true
 */
void show_verbose(const std::string &line)
{
	if (enableVerbose)
	{
		std::cerr << line;
	}
}


/**
 * Function: Show output to std::cerr whenever enableDebug is true
 */
void show_debug(const std::string &line)
{
	if (enableDebug)
	{
		std::cerr << line;
	}
}


/**
 * Function: Pass formatted output to given stream whenever "verbose" is
 * enabled.
 */
void verbose_bad(std::ostream &os, const char *msg, ...)
{
	if (enableVerbose)
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
 * Function: Pass formatted output to given stream whenever "debug" is
 * enabled.
 */
void verbose_good(std::ostream &os, const char *msg, ...)
{
	if (enableDebug)
	{
		char buf[1024];

		va_list ap;
		va_start(ap, msg);
		vsprintf(buf, msg, ap);
		va_end(ap);

		os << buf;
	}
}
