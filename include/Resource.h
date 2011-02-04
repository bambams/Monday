#ifndef Resource_h
#define Resource_h


#include <string>


class Resource
{
	public:
		virtual ~Resource(){}
		virtual bool Load(const std::string filename) = 0;
};


#endif  //  Resource_h
