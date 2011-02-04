#ifndef Parse_Lines_h
#define Parse_Lines_h


#include <fstream>
#include <string>


/* Scan file for non-comment lines */
bool seek_non_comment_line(std::ifstream &fs, std::string &line);


#endif  //  Parse_Lines_h
