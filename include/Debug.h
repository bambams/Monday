#ifndef Debug_h
#define Debug_h


#include "StdTypes.h"

#include <iostream>
#include <string>
#include <cstring>

/* NOTE: If you add or remove any items here, update "debugOn()" in Debug.cpp
 * to reflect the modifications.
 */
enum LevelDefinitions
{
	/* Graceful recovery from these kinds of errors are impossible; these are
	 * show-stoppers and must be fixed ASAP.  This message should immediately
	 * be followed by a call to exit().
	 */
	CRITICAL_LEVEL = 0,

	/* Graceful recovery from these kinds of errors are possible, though the
	 * game cannot proceed forward from this point.  This includes missing
	 * files and resources, corrupt data files, etc.  The programmer can at
	 * least give useful information about the cause of the error before the
	 * program bails.
	 */
	ERROR_LEVEL,

	/* Expected but non-critical files and data, like those found in config
	 * files, cannot be found or loaded.  Defaults can usually be used and
	 * the game can continue from this point, but a programmer or developer
	 * should look at the issue (things missing in DEBUG builds may be okay,
	 * but this should be resolved by the time a RELEASE build is made).
	 */
	WARNING_LEVEL,

	/* Hints and reminders to programmers/developers can be made at this
	 * level.  This may include messages such as "FIXME: player's sprite is
	 * not being warped to the correct position..."  No recovery need be made
	 * for this error level.
	 */
	SUGGESTION_LEVEL,

	/* When testing to ensure things are loading/working correctly, use these
	 * values.  Add to the TOP of the VERBOSE_LEVEL# list so older not-as-
	 * important debugging can be ignored while you work out the "new problems"
	 * and avoid excessive outputs (warning: all of these VERBOSE levels
	 * generate a LOT of output (several KB to MB).  Use sparingly.
	 */
	VERBOSE_LEVEL4,
	VERBOSE_LEVEL3,
	VERBOSE_LEVEL2,
	VERBOSE_LEVEL1,

	/* Must be the last item in the enum so we know how many items have been
	 * defined here.
	 */
	NUM_DEBUG_DEFINITIONS
};


/* Function: debugOn
 * Turn on debugging output when showDebug is > level.
 */
void debugOn(std::ostream& os, const int& level);


/* Function: debugOff
 * Turn off debugging output.
 */
void debugOff(std::ostream& os);


/* Macro: mon_assert
 * Makes sure an expression is true.
 * If not, the expression and the location of it is printed.
 */
#define mon_assert(e) { if (!(e)) { Monday_assert(#e, __FILE__, __LINE__); } }

/* Macro: printlog
 * Prints a message along with the location from where it was called.
 */
#define printlog(msg) Monday_log(msg, __FILE__, __LINE__);

/* Function: Monday_assert
 * If condition does not evaluate true, dump file and line where
 * condition failed, and exit program.
 */
void Monday_assert(const char* e, const char* file, int line);

/* Function: Monday_log
 * Display the given message along with the current filename and line.
 */
void Monday_log(const std::string& e, const char* file, int line);


/* Display a debugging message at a certain debugging level */
void Monday_out(const u32 &debugLevel, std::ostream& os, const char* msg, ...);


/* Display a map containing a "name = value" pair */
void Show_attributes(std::ostream& os, Attributes& attributes);

/* Debug message level, specified on command-line */
extern u32 showDebug;


#endif  //  Debug_h
