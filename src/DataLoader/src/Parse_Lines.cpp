#include "Parse_Lines.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <fstream>
#include <string>
#include <iostream>
#include <iomanip>


/**
 * Function: Search the given input file stream, ignoring line comments.
 *
 * The "#"-sign needs to be the first symbol found on a given line (ignoring
 * whitespace) for the entire line to be ignored as a comment.
 *
 * Parameter: fs Opened file input stream from whence we pull the data to read
 * Parameter: line Reference to string object which will receive the next non-
 *                 comment line
 * Returns: TRUE if a non-comment (and non-blank) line is found, false otherwise
 */
bool seek_non_comment_line(std::ifstream &fs, std::string &line)
{
	while (!fs.bad() && !fs.eof())
	{
		std::getline(fs, line);

		/* Allows for whitespace preceding the line comment's "#" indicator */
		u32 indicator_position = line.find_first_not_of(" \t");
		if ((indicator_position != std::string::npos && line[indicator_position] == '#') || (line == ""))
		{
			/* This line is a comment (or is blank); disregard it. */
			continue;
		}
		else
		{
			/* We found a value whose first character is not "#" (or whitespace) */
			return true;
		}
	}

	/* We'll only get here if bad input (like EOF) occurs before we were
	 * expecting it to.  Erase "line" so caller doesn't get garbage values.
	 */
	line = "";
	return false;
}
