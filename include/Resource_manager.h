#ifndef Resource_Manager_h
#define Resource_Manager_h


#include <map>
#include <string>
#include <typeinfo>
#include <iostream>

#include "Resource.h"

typedef std::map <std::string, Resource*> Resource_map;

/* Class: Resource_manager
 * Managing resources.
 * */
class Resource_manager
{
    public:
    	/* Constructor: Resource_manager
    	 * */
        Resource_manager();
    	/* Destructor: Resource_manager
    	 * */
        ~Resource_manager();

		/* Function: Load
		 * Loads a resource, or returns it if it has already been loaded.
		 * 
		 * Parameters:
		 * filename - File to load
		 * 
		 * Returns:
		 * Pointer to loaded resource.
		 * */
        template <typename T> T *Load(std::string filename)
		{
            std::string resname = filename + "*" + typeid(T).name();
            Resource_map::iterator i = resources.find(resname);

            if(i != resources.end()) {
                return (T*)(i->second);
            }

            T *resource = new T;
            if(!resource->Load(filename)) {
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


#endif  //  Resource_Manager_h
