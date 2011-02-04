#include "Animation.h"
#include <iostream>
#include <fstream>

bool Animation::Load(std::string filename)
{
	std::string folder;
	size_t pos = filename.find_last_of('/');
	if(pos!=std::string::npos)
		folder = filename.substr(0, pos+1);
	
	std::ifstream file;
	file.open(filename.c_str());

	bool loading = false;
	while(!file.eof())
	{
		std::string line;
		std::getline(file, line);

		if(line == "start")
		{
			loading = true;
			continue;
		}
		if(line == "end")
			loading = false;
			
		if(loading)
		{
			ALLEGRO_BITMAP *bitmap;
			bitmap = al_iio_load((folder+line).c_str());
			if(!bitmap)
			{
				std::cout<<line<<" failed to load"<<std::endl;
			}
			else
				frames.push_back(bitmap);
		}
	}

	file.close();
	return true;
}

Animation::~Animation()
{
	for(Frames::iterator i = frames.begin(); i!=frames.end(); ++i)
	{
		al_destroy_bitmap(*i);
	}
}

double Animation::Frame_time()
{
	return .3;
}

int Animation::Number_of_frames()
{
	return frames.size();
}

void Animation::Render_frame(int frame, float x, float y, float scale)
{
	//al_draw_bitmap(frames[frame], x, y, 0);
/*	al_draw_scaled_bitmap(
		frames[frame], 
		0, 0, frames[frame]->w, frames[frame]->h,
		x, y, frames[frame]->w, frames[frame]->h, 
		0
	)
*/
//frames[frame]->w/2 frames[frame]->h/2
	al_draw_rotated_scaled_bitmap(frames[frame], 0, 0, x, y, scale, scale, 0, 0);
}
