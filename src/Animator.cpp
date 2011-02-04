#include "Animator.h"

void Animator::Set_animation(Animation* ianimation)
{
	animation = ianimation;
	delta=0;
	frame=0;
}

void Animator::Update(double idelta)
{
	delta+=idelta;
	while(animation->Frame_time()<=delta)
	{
		delta-=animation->Frame_time();
		++frame;
		if(frame >= animation->Number_of_frames())
			frame=0;
	}
}

void Animator::Render(float x, float y, float scale)
{
	animation->Render_frame(frame, x, y, scale);
}
