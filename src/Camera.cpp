#include "Camera.h"
#include <cstdlib>
#include <iostream>

void Camera::Set_position(Vector p)
{
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
	effect_sum += Vector(rand()/float(RAND_MAX) * rumble, rand()/float(RAND_MAX) * rumble);
	apply = viewport_size/2 - position + effect_sum;
}

void Camera::Set_rumble(float amount)
{
	rumble = amount;
	std::cout<<"Rumble: "<<rumble<<std::endl;
}
