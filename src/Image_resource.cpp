#include "Image_resource.h"

Image_resource::Image_resource()
:bitmap(NULL)
{
}

Image_resource::~Image_resource()
{
	al_destroy_bitmap(bitmap);
}

bool Image_resource::Load(const std::string filename)
{
	bitmap = al_iio_load(filename.c_str());
	if(!bitmap)
	{
		return false;
	}
	else
	{
		return true;
	}
}

ALLEGRO_BITMAP* Image_resource::Get_allegro_bitmap()
{
	return bitmap;
}
