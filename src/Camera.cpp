#include "Camera.h"

#include <cstdlib>
#include <iostream>


Camera::Camera()
:rumble(0)
{
}


void Camera::Set_position(Vector p)
{
//	rumble = 0;
	position = p;
	apply = viewport_size/2 - position + effect_sum;
}


Vector Camera::Get_position()
{
	return position;
}


Vector Camera::Apply(const Vector& v) const
{
	return v+apply;
}


void Camera::Set_viewport(Vector topleft, Vector size)
{
	viewport_size = size;
	viewport_topleft = topleft;
}


void Camera::Update(float dt)
{
	effect_sum.Set(0, 0);
	float precalc = (float)rumble / (float)RAND_MAX;
	effect_sum += Vector(rand() * precalc, rand() * precalc);
	apply = viewport_size/2 - position + effect_sum;
}


void Camera::Set_rumble(float amount) // Do we ever use floats for this, or would ints work?
{
	rumble = amount;
	Monday_out(0, std::cout, "Rumble: %.f\n", rumble); // should use Monday_out to keep code consistent
}

