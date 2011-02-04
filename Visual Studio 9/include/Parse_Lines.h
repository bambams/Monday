#ifndef Parse_Lines_h
#define Parse_Lines_h


#include <fstream>
#include <string>
#include <sstream>
#include <vector>


/* Scan file for non-comment lines */
bool seek_non_comment_line(std::ifstream& fs, std::string& line);

/* Find a "name=value" pair within the specified string */
bool name_and_value(const std::string& str, std::string& name, std::vector<std::string>& manyValues);

/* Splits strings into individual tokens, separated by *delimiters* */
std::vector<std::string> tokenize(const std::string& str, const std::string& delimiters);

/* Get the next token from the list */
std::string next_token(const std::string& str, const std::string& delimiters, std::string::size_type& pos);


template <class T>
T String_to_type(const std::string& s)
{
    std::stringstream ss;
    ss << s;
    T t;
    ss >> t;
    return t;
}


template <class T>
std::string Type_to_string(const T& t)
{
    std::stringstream ss;
    ss << t;
    std::string s;
    ss >> s;
    return s;
}


#endif  //  Parse_Lines_h
