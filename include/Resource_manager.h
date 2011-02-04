#ifndef Resource_manager_h
#define Resource_manager_h


#include "Resource.h"

#include <map>
#include <string>
#include <typeinfo>
#include <iostream>

typedef std::map < std::string, Resource *> Resource_map;

class Resource_manager
{
	public:
		Resource_manager();
		~Resource_manager();

		template <typename T> T *Load(std::string filename)
		{
			std::string resname = filename + "*" + typeid(T).name();
			Resource_map::iterator i = resources.find(resname);

			if (i != resources.end())
			{
				return (T*)(i->second);
			}

			T *resource = new T;
			if (!resource->Load(filename))
			{
				std::cout << "Resource failed to load: " << filename << std::endl;
				delete resource;
				return 0;
			}

			resources[resname] = resource;

			return resource;
		}

	private:
		Resource_map resources;
};


#endif  //  Resource_manager_h
