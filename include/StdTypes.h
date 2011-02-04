#ifndef StdTypes_h
#define StdTypes_h

#include <map>
#include <string>


/* Shorthand for these standard types */
typedef unsigned int   u32;
typedef signed int     s32;
typedef unsigned short u16;
typedef signed short   s16;
typedef unsigned char  u8;
typedef signed char    s8;


/* This contains a pair of "name=value" strings */
typedef std::map<std::string, std::string> Attributes;


/* Worlds, Maps, Levels, etc. are all identified with an ID */
typedef std::string id_type;


#endif  //  StdTypes_h
