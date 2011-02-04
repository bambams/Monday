#ifndef Animation_h
#define Animation_h

#include <vector>
#include <string>
#include <allegro5/allegro5.h>

typedef std::vector<ALLEGRO_BITMAP*> Frames;

class Animation
{
public:
	~Animation();
	void Add_frame(ALLEGRO_BITMAP* bitmap);
	double Frame_time();
	int Number_of_frames();
	void Render_frame(int frame, float x, float y, float scale, bool hflip);
private:
	double frame_time;
	Frames frames;
};

#endif
