#ifndef Camera_h
#define Camera_h


#include "Vector.h"


class Camera
{
	public:
		Camera();

		/*
		 * Position is the center of view. So an object at position will be in the middle of the viewport.
		 */
		void Set_position(Vector p);
		void Set_viewport(Vector topleft, Vector size);
		Vector Get_position();
		Vector Apply(const Vector &v) const;
		void Update(float dt);

		void Set_rumble(float amount);
		float Get_rumble();

	private:
		Vector position;
		Vector viewport_size;
		Vector viewport_topleft;
		Vector apply;

		Vector effect_sum;

		float rumble;
};


#endif  //  Camera_h
