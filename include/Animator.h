#ifndef Animator_h
#define Animator_h

#include "Animation.h"

class Animator
{
public:
	void Set_animation(Animation* animation);
	void Update(double delta);
	void Render(float x, float y, float scale);
private:
	double delta;
	int frame;
	Animation* animation;
};

#endif
