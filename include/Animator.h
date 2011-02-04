#ifndef Animator_h
#define Animator_h

#include "Animation.h"
#include <map>
#include "Vector.h"
#include "Resource_manager.h"

typedef std::map<std::string, Animation*> Animations;

class Animator
{
public:
	Animator();
	void Set_animations(Animations a);
	void Set_animation(const std::string& a, bool hflip = false);
	void Update(double delta);
	void Render(float x, float y, float scale);
	void Render(const Vector& position, float scale);
private:
	double delta;
	int frame;
	Animations animations;
	Animation* animation;
	bool hflip;
};

Animations Load_animations(const std::string& filename, Resource_manager& resource_manager);

#endif
