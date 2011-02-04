#include "Animation.h"
#include <iostream>
#include <fstream>
#include "Debug.h"

void Animation::Add_frame(ALLEGRO_BITMAP* bitmap)
{
	assert(bitmap)
	frames.push_back(bitmap);
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
	return 0.3;
}

int Animation::Number_of_frames()
{
	return frames.size();
}

void Animation::Render_frame(int frame, float x, float y, float scale, bool hflip)
{
	if(frame<(int)frames.size())
	{
		assert(frames[frame])
		al_draw_rotated_scaled_bitmap(frames[frame], al_get_bitmap_width(frames[frame])/2, al_get_bitmap_height(frames[frame])/2, x, y, scale*(hflip?-1:1), scale, 0, 0);
	}
}
