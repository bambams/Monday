#include "Vector.h"
#include <cmath>

Vector::Vector()
	: x(0),
	  y(0)
{
	// Nothing to do
}


Vector::Vector(float ix, float iy)
	: x(ix),
	  y(iy)
{
	// Nothing to do
}


void Vector::Set(float ix, float iy)
{
	x = ix;
	y = iy;
}


const Vector &Vector::operator = (const Vector &v)
{
	x = v.x;
	y = v.y;
	return *this;
}


bool Vector::operator != (const Vector &v) const
{
	return !(operator == (v));
}


bool Vector::operator == (const Vector &v) const
{
	if (x == v.x && y == v.y)
	{
		return true;
	}
	return false;
}


float Vector::X() const
{
	return x;
}


float Vector::Y() const
{
	return y;
}


float Vector::Length() const
{
	/* It's expensive to push another function onto the stack in order to
	 * do this simple math; just manually calculate it.
	 */
	return sqrt(x * x + y * y);
}


float Vector::Length_squared() const
{
	return (x * x + y * y);
}


void Vector::Normalize()
{
	/* The only way "sqrt(x^2 + y^2)" would return 0 is if x == y */
	if (x - y != 0)
	{
		float length = 1.0f / sqrtf(x * x + y * y);
		x *= length;
		y *= length;
	}
}


Vector Vector::Normalized() const
{
	/* The only way "sqrt(x^2 + y^2)" would return 0 is if x == y */
	if (x - y != 0)
	{
		float length = 1.0f / sqrtf(x * x + y * y);
		return Vector(x * length, y * length);
	}
	return *this;
}


float Vector::Dotproduct(const Vector &v) const
{
	return x * v.x + y * v.y;
}


const Vector &Vector::operator += (const Vector &v)
{
	x += v.x;
	y += v.y;
	return *this;
}


const Vector &Vector::operator -= (const Vector &v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}


const Vector &Vector::operator *= (float v)
{
	x *= v;
	y *= v;
	return *this;
}


const Vector &Vector::operator /= (float v)
{
	if (v != 0)
	{
		x /= v;
		y /= v;
	}
	return *this;
}


const Vector Vector::operator - (const Vector &v) const
{
	return Vector(x - v.x, y - v.y);
}


const Vector Vector::operator + (const Vector &v) const
{
	return Vector(x + v.x, y + v.y);
}


void Vector::Zero()
{
	x = 0;
	y = 0;
}


Vector Vector::operator / (float s) const
{
	Vector temp(0, 0);

	/* Prevent divide-by-zero */
	if (s != 0)
	{
		temp.Set(x / s, y / s);
	}

	return temp;
}


Vector Vector::operator * (float s) const
{
	Vector temp(0, 0);

	/* Prevent divide-by-zero */
	if (s != 0)
	{
		temp.Set(x * s, y * s);
	}

	return temp;
}


float Vector::Angle() const
{
	return atan2(y, x);
}
