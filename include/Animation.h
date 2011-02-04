#ifndef Animation_h
#define Animation_h


#include "main.h"

#include <vector>
#include <string>


typedef std::vector<ALLEGRO_BITMAP *> Frames;

class Animation
{
	public:
		Animation();
		virtual ~Animation();
		void Add_frame(ALLEGRO_BITMAP *bitmap);
		double Frame_time();
		int Number_of_frames();
		void Render_frame(int frame, float x, float y, float scale, bool hflip);

	private:
		double frame_time;
		Frames frames;
};


#endif  //  Animation_h
