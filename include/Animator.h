#ifndef Animator_h
#define Animator_h

#include "Animation.h"
#include <map>
#include "Vector.h"
#include "Resource_manager.h"

typedef std::map<std::string, Animation*> Animations;

/* Class: Animator
 * Handles playback of animations.
 */
class Animator
{
public:
	/* Constructor: Animator
	 */
	Animator();
	/* Function: Set_animations
	 * Parameters:
	 * a - A set of animations for this animator to handle.
	 * */
	void Set_animations(Animations a);
	/* Function: Set_animation
	 * Tells animator which animation to show.
	 * 
	 * Parameters:
	 * a - String identifier of an animation handled by the animator.
	 * hflip - For rendering the animation with horizontal flip.
	 * */
	void Set_animation(const std::string& a, bool hflip = false);
	/* Function: Update
	 * Parameters:
	 * delta - The amount of time passed since last Update.
	 * */
	void Update(double delta);
	/* Function: Render
	 * Parameters:
	 * x, y - Where to render the animation, centered.
	 * scale - scale
	 * */
	void Render(float x, float y, float scale);
	/* Function: Render
	 * Same as the other Render, but takes a Vector for position.
	 * */
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
