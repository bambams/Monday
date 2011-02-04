#include "Debug.h"
#include <iostream>
#include <stdlib.h>

void Monday_assert(const char *e, const char *file, int line)
{
	std::cerr << "assert(" << e << ") failed at " << file << ":" << line << std::endl;
	exit(0);
}
