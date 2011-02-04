#ifndef Camera_h
#define Camera_h

#include "Vector.h"
#include "Debug.h"

/* Class: Camera
 * Manages a viewpoint to provide an offset for rendering.
 * */
class Camera
{
public:
	/* Function: Default constructor, no arguments
	 */
	Camera();

	/* Function: Set_position
	 * Position is the center of view. So an object at position will be in the middle of the viewport.
	 *
	 * Parameters:
	 * p - The position
	*/
	void Set_position(Vector p);
	/* Function: Set_viewport
	 *
	 * Parameters:
	 * topleft - The top left corner on screen.
	 * size - Width and height of the viewport.
	 * */
	void Set_viewport(Vector topleft, Vector size);
	/* Function: Get_position
	 * Returns:
	 * The point this camera is looking at.
	 * */
	Vector Get_position();
	/* Function: Apply
	 * Applies the camera's view to a point.
	 *
	 * Parameters:
	 * v - Position to offset
	 *
	 * Returns:
	 * Position with applied offset.
	 * */
	Vector Apply(const Vector& v) const;
	/* Function: Update
	 * Updates effects in the camera.
	 *
	 * Parameters:
	 * dt - Time passed since last update.
	 * */
	void Update(float dt);

	void Set_rumble(float amount);
private:
	Vector position;
	Vector viewport_size;
	Vector viewport_topleft;
	Vector apply;

	Vector effect_sum;

	float rumble;
};

#endif
