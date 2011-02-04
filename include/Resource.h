#ifndef __resource_h__
#define __resource_h__

#include <string>

class Resource {
    public:
        virtual ~Resource(){}
        virtual bool Load(const std::string filename) = 0;
};

#endif
