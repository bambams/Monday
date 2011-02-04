#include "Animator.h"

#include "Debug.h"
#include "Image_resource.h"

#include <fstream>
#include <iostream>


Animator::Animator()
:delta(0)
,frame(0)
,animation(NULL)
,hflip(false)
{
}


void Animator::Set_animations(Animations a)
{
	animations = a;
//	animation = animations.begin()->second;
}


void Animator::Set_animation(const std::string& a, bool ihflip)
{
	hflip = ihflip;
	if(animation != animations[a])
	{
		animation = animations[a];
		delta=0;
		frame=0;
	}
}


void Animator::Update(double idelta)
{
	if(!animation)
		return;
	delta+=idelta;
	while(animation->Frame_time()<=delta)
	{
		delta-=animation->Frame_time();
		++frame;
		if(frame >= animation->Number_of_frames())
			frame=0;
	}
}


void Animator::Render(const Vector& position, float scale)
{
	Render(position.X(), position.Y(), scale);
}


void Animator::Render(float x, float y, float scale)
{
	if(animation)
		animation->Render_frame(frame, x, y, scale, hflip);
}


Animations Load_animations(const std::string& filename, Resource_manager& resource_manager)
{
	std::string folder;
	size_t pos = filename.find_last_of('/');
	if(pos!=std::string::npos)
		folder = filename.substr(0, pos+1);

	std::ifstream file;
	file.open(filename.c_str());

	Animations animations;
	Animation* animation = NULL;
	std::string name;

	bool loading = false;
	while(!file.eof())
	{
		std::string line;
		std::getline(file, line);

		if(line == "start")
		{
			animation = new Animation;
			std::getline(file, line);
			name = line;
			loading = true;
			continue;
		}
		if(line == "end")
		{
			animations[name] = animation;
			loading = false;
		}

		if(loading)
		{
			Monday_out(VERBOSE_LEVEL1, std::cout, "Animations Load_animations()\n\tfolder: \"%s\"\n\tline: \"%s\"\n", folder.c_str(), line.c_str());
			Image_resource* image = resource_manager.Load<Image_resource>(folder+line);
			if(image)
			{
				animation->Add_frame(image->Get_allegro_bitmap());
			}
		}
	}

	file.close();
	return animations;
}
