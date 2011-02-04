#ifndef Animation_h
#define Animation_h

#include <vector>
#include <string>
#include <allegro5/allegro5.h>

typedef std::vector<ALLEGRO_BITMAP*> Frames;
/*
 * Class: Animation
 * Holds data for an animation
*/
class Animation
{
public:
	/* Function: Add_frame
	 * Parameters:
	 * bitmap - Bitmap to add, does not accept NULL.
	 * */
	void Add_frame(ALLEGRO_BITMAP* bitmap);
	/* Function: Frame_time
	 * Returns:
	 * Time that a frame should be visible before going to the next frame.
	 * */
	double Frame_time();
	/* Function: Number_of_frames
	 * Returns:
	 * Number of frames in the animation.
	 */
	int Number_of_frames();
	/* Function: Render_frame
	 * Parameters:
	 * frame - Which frame to render
	 * x, y - Where to render it, image is centered.
	 * scale - scale
	 * hflip - Horizontal flip
	 */
	void Render_frame(int frame, float x, float y, float scale, bool hflip);
private:
	double frame_time;
	Frames frames;
};

#endif
