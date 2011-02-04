#ifndef __resource_h__
#define __resource_h__

#include <string>

/* Class: Resource
 * Base class for resources.
 * */
class Resource {
    public:
    	/* Destructor: Resource
    	 * */
        virtual ~Resource(){}
        /* Function: Load
         * Loads the resource.
         * 
         * Parameters:
         * filename - The file to load.
         * 
         * Returns:
         * true on success, false on failure.
         * */
        virtual bool Load(const std::string filename) = 0;
};

#endif
