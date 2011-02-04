#include "Vector.h"
#include <cmath>

Vector::Vector()
:x(0), y(0)
{
}

Vector::Vector(float ix, float iy)
:x(ix), y(iy)
{}

void Vector::Set(float ix, float iy)
{
	x=ix;
	y=iy;
}

const Vector& Vector::operator=(const Vector& v)
{
	x = v.x;
	y = v.y;
	return *this;
}

bool Vector::operator!=(const Vector& v) const
{
	return !operator==(v);
}

bool Vector::operator==(const Vector& v) const
{
	if(x==v.x && y==v.y)
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
	return sqrt(Length_squared());
}

float Vector::Length_squared() const
{
	return x*x+y*y;
}

void Vector::Normalize()
{
	float length=Length();
	if(length!=0)
	{
		x/=length;
		y/=length;
	}
}

Vector Vector::Normalized() const
{
	float length=Length();
	if(length!=0)
	{
		return Vector(x/length, y/length);
	}
	return *this;
}

float Vector::Dotproduct(const Vector& v) const
{
	return x * v.x + y * v.y;
}

const Vector& Vector::operator+=(const Vector& v)
{
	x+=v.x;
	y+=v.y;
	return *this;
}

const Vector& Vector::operator-=(const Vector& v)
{
	x-=v.x;
	y-=v.y;
	return *this;
}

const Vector& Vector::operator*=(float v)
{
	x*=v;
	y*=v;
	return *this;
}

const Vector& Vector::operator/=(float v)
{
	x/=v;
	y/=v;
	return *this;
}

const Vector Vector::operator-(const Vector& v) const
{
	return Vector(x-v.x, y-v.y);
}

const Vector Vector::operator+(const Vector& v) const
{
	return Vector(x+v.x, y+v.y);
}

void Vector::Zero()
{
	x=0;
	y=0;
}

Vector Vector::operator/(float s) const
{
	Vector temp;
	temp.Set(x/s, y/s);
	return temp;
}

Vector Vector::operator*(float s) const
{
	Vector temp;
	temp.Set(x*s, y*s);
	return temp;
}

float Vector::Angle() const
{
	return atan2(y, x);
}
