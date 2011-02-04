#include "Resource_manager.h"


Resource_manager::Resource_manager()
{}


Resource_manager::~Resource_manager()
{
	for(Resource_map::iterator i = resources.begin(); i != resources.end(); ++i)
	{
		delete (*i).second;
	}
}
