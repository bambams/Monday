#ifndef Debug_h
#define Debug_h


#include <iostream>
#include <iomanip>
#include <string>


#define assert(e) { if (!(e)) { Monday_assert(#e, __FILE__, __LINE__); } }

void Monday_assert(const char *e, const char *file, int line);


/* Show output to std::cerr whenever enableVerbose is true */
void show_verbose(const std::string &line);

/* Show output to std::cerr whenever enableDebug is true */
void show_debug(const std::string &line);

/* Show formatted output through std::cerr whenever enableVerbose is true */
void verbose_bad(std::ostream &os, const char *msg, ...);

/* Show formatted output through std::cerr whenever enableDebug is true */
void verbose_good(std::ostream &os, const char *msg, ...);


extern bool enableDebug;
extern bool enableVerbose;


#endif  //  Debug_h
