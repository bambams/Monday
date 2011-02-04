#include "Image_resource.h"

Image_resource::Image_resource()
	: bitmap(NULL)
{
	// Nothing to do
}


Image_resource::~Image_resource()
{
	al_destroy_bitmap(bitmap);
}


bool Image_resource::Load(const std::string &filename)
{
	bitmap = al_iio_load(filename.c_str());
	return (bitmap != NULL);
}


ALLEGRO_BITMAP *Image_resource::Get_allegro_bitmap()
{
	return bitmap;
}
