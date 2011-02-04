#ifndef Image_resource_h
#define Image_resource_h

#include "Resource.h"
#include <allegro5/allegro5.h>
#include <allegro5/a5_iio.h>

class Image_resource: public Resource
{
public:
	Image_resource();
	~Image_resource();
	virtual bool Load(const std::string filename);
	ALLEGRO_BITMAP* Get_allegro_bitmap();
private:
	ALLEGRO_BITMAP* bitmap;
};

#endif
