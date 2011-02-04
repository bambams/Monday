#ifndef Animation_h
#define Animation_h

#include <vector>
#include <string>
#include <allegro5/allegro5.h>
#include <allegro5/a5_iio.h>

typedef std::vector<ALLEGRO_BITMAP*> Frames;

class Animation
{
public:
	~Animation();
	bool Load(std::string filename);
	double Frame_time();
	int Number_of_frames();
	void Render_frame(int frame, float x, float y, float scale);
private:
	double frame_time;
	Frames frames;
};

#endif
