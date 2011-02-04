#ifndef Debug_h
#define Debug_h

#define assert(e) if(!(e))Monday_assert(#e, __FILE__, __LINE__);

void Monday_assert(const char* e, const char* file, int line);

#endif
