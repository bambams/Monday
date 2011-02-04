#ifndef Image_resource_h
#define Image_resource_h

#include "Resource.h"
#include <allegro5/allegro5.h>
#include <allegro5/a5_iio.h>

/* Class: Image_resource
 * */
class Image_resource: public Resource
{
public:
	/* Constructor: Image_resource
	 * */
	Image_resource();
	/* Destructor: Image_resource
	 * */
	~Image_resource();
	/* Function: Load
	 * Loads image from file.
	 * 
	 * Returns:
	 * true on success
	 * */
	virtual bool Load(const std::string filename);
	/* Function: Get_allegro_bitmap
	 * returns:
	 * The image.
	 * */
	ALLEGRO_BITMAP* Get_allegro_bitmap();
private:
	ALLEGRO_BITMAP* bitmap;
};

#endif
