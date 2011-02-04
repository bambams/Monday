#include "Animator.h"
#include "Image_resource.h"

#include <iostream>
#include <fstream>

Animator::Animator()
	: animation(NULL)
{
	delta = 0;
	frame = 0;
	hflip = false;
}


void Animator::Set_animations(const Animations &a)
{
	animations = a;
	animation = animations.begin()->second;
}


void Animator::Set_animation(const std::string &a, bool ihflip)
{
	hflip = ihflip;
	if (animation != animations[a])
	{
		animation = animations[a];
		delta = 0;
		frame = 0;
	}
}


void Animator::Update(double idelta)
{
	if (!animation)
		return;
	delta += idelta;
	double frame_time = animation->Frame_time();
	while (frame_time <= delta)
	{
		delta -= frame_time;
		++frame;
		if (frame >= animation->Number_of_frames())
		{
			frame = 0;
		}
		/* Will this really be recalculated each time, or will it always give
		 * us the same value back?
		 */
		frame_time = animation->Frame_time();
	}
}


void Animator::Render(const Vector &position, float scale)
{
	Render(position.X(), position.Y(), scale);
}


void Animator::Render(float x, float y, float scale)
{
	if (animation)
	{
		animation->Render_frame(frame, x, y, scale, hflip);
	}
}


/* OC: This is just a note to myself, because I keep forgetting this point:
 * This function is not part of the Animator class, so "Animations animations"
 * and "Animation *animation" are FINE to be declared here!
 */
Animations Load_animations(const std::string &filename, Resource_manager *resource_manager)
{
	std::string folder;
	size_t pos = filename.find_last_of('/');
	if (pos != std::string::npos)
	{
		folder = filename.substr(0, pos + 1);
	}

	std::ifstream file;
	file.open(filename.c_str());
	if (!file.is_open())
	{
		al_trace("I don't think the file opened...\n");
	}

	Animations animations;
	Animation *animation = NULL;
	std::string name;

	bool loading = false;
	while (!file.eof())
	{
		std::string line;
		std::getline(file, line);

		if (line == "start")
		{
			animation = new Animation;
			std::getline(file, line);
			name = line;
			loading = true;
			continue;
		}
		if (line == "end")
		{
			animations[name] = animation;
			loading = false;
		}

		if (loading && resource_manager != NULL)
		{
			Image_resource *image = resource_manager->Load<Image_resource>(folder + line);
			if (image)
			{
				animation->Add_frame(image->Get_allegro_bitmap());
			}
		}
	}

	file.close();
	return animations;
}
