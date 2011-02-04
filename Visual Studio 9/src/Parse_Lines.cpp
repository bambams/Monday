#include "Parse_Lines.h"

#include "Debug.h"

/* Defines u32, s32, u16, s16, u8, s8, etc. */
#include "StdTypes.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>


/**
 * Function: Search the given input file stream, ignoring line comments.
 *
 * The "#"-sign needs to be the first symbol found on a given line (ignoring
 * whitespace) for the entire line to be ignored as a comment.
 *
 * Parameter: -fs - Opened file input stream from whence we pull the data
 * Parameter: line - Reference to string object which will receive the next non-
 *                   comment line
 * Returns: TRUE if a non-comment (and non-blank) line is found, false otherwise
 */
bool seek_non_comment_line(std::ifstream& fs, std::string& line)
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


/**
 * Function: Finds a "name = value" pair within the specified string.
 *
 * Note that "value" may be comma-separated: "file1, file2, file3"
 * Example:
 *    animations = file1.png, file2.png, file3.png, ..., file99.png
 *
 * Parameter: str - Line from which to search for the "name = value" pair
 * Parameter: name - (reference) "name" found within *str* string
 * Parameter: manyValues - (reference) array of "values" found within *str* string
 *
 * Returns: false if no "name = value" pair found, else true on success
 */
bool name_and_value(const std::string& str, std::string& name, std::vector<std::string>& manyValues)
{
	/* This should split everything to the left of "=" into one token and
	 * everything to the right into a second.
	 */
	std::vector<std::string> tokens = tokenize(str, "=");
	if (tokens.size() != 2)
	{
		/* We must not have found a "=", or we found more than one (oops).
		 */

		/* Print some debugging info */
		u32 size = tokens.size();
		for (u32 i = 0; i < size; ++i)
		{
			Monday_out(VERBOSE_LEVEL1, std::cout, "Token %d: \"%s\"\n", i, tokens[i].c_str());
		}

		return false;
	}

	/* Strip out whitespace surrounding "name" and return the cleaned-up string
	 * to the caller.
	 */
	std::vector<std::string> nameTokens = tokenize(tokens[0], " \t");
	name = nameTokens[0];
	u32 nameTokensSize = nameTokens.size();
	for (u32 i = 1; i < nameTokensSize; ++i)
	{
		name = name + " " + nameTokens[i];
	}

	/* For the "values", we need to do this in stages.
	 * Stage 1: Separate by commas
	 * Stage 2: Remove whitespace around each tokenized string
	 * Stage 3: Return the cleaned-up vector to the caller
	 */

	// Stage 1: commas
	std::vector<std::string> valueTokens = tokenize(tokens[1], ",");
	u32 valueTokensSize = valueTokens.size();
	for (u32 i = 0; i < valueTokensSize; ++i)
	{
		// Stage 2: whitespace
		std::vector<std::string> cleaned = tokenize(valueTokens[i], " \t");
		std::string cleanedString = cleaned[0];
		u32 cleanedSize = cleaned.size();
		for (u32 cleanIndex = 1; cleanIndex < cleanedSize; ++cleanIndex)
		{
			cleanedString = cleanedString + " " + cleaned[cleanIndex];
		}

		// Stage 3: return cleaned-up string to "value" vector
		if ("" != cleanedString)
		{
			manyValues.push_back(cleanedString);
		}
	}

	u32 valueSize = manyValues.size();
	Monday_out(VERBOSE_LEVEL1, std::cout, "\tName: \"%s\"\n", name.c_str());
	for (u32 i = 0; i < valueSize; ++i)
	{
		Monday_out(VERBOSE_LEVEL1, std::cout, "\t\tValue %d: \"%s\"\n", i, manyValues[i].c_str());
	}

    return true;
}


//Function from http://www.digitalpeer.com/id/simple
std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters)
{
	std::vector<std::string> tokens;

	// skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

	// find first "non-delimiter".
	std::string::size_type pos = str.find_first_of(delimiters, lastPos);

	while (std::string::npos != pos || std::string::npos != lastPos)
	{
		// found a token, add it to the vector.
		std::string temp = str.substr(lastPos, pos - lastPos);
		if ("" != temp)
		{
			tokens.push_back(temp);
		}

		// skip delimiters.  Note the "not_of"
		lastPos = str.find_first_not_of(delimiters, pos);

		// find next "non-delimiter"
		pos = str.find_first_of(delimiters, lastPos);
	}

	return tokens;
}


std::string next_token(const std::string& str, const std::string& delimiters, std::string::size_type& pos)
{
	// skip delimiters at beginning.
	std::string::size_type lastPos = str.find_first_not_of(delimiters, pos);

	// find first "non-delimiter".
	pos = str.find_first_of(delimiters, lastPos);

	if (std::string::npos != pos || std::string::npos != lastPos)
	{
		// found a token, return it
		return str.substr(lastPos, pos - lastPos);
	}

	return "";
}
