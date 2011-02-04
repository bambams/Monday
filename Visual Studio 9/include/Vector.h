#ifndef __VECTOR_H
#define __VECTOR_H

/* Class: Vector
 * A 2D vector.
 * */
class Vector
{
public:
	/* Constructor: Vector
	 * */
	Vector();
	/* Constructor: Vector
	 * */
	Vector(float x, float y);
	/* Operator: operator=
	 * */
	const Vector& operator=(const Vector& v);
	/* Operator: operator==
	 * */
	bool operator==(const Vector& v) const;
	/* Operator: operator!=
	 * */
	bool operator!=(const Vector& v) const;
	/* Function: Set
	 * */
	void Set(float x, float y);
	/* Function: X
	 * */
	float X() const;
	/* Function: Y
	 * */
	float Y() const;
	/* Function: Zero
	 * Sets the vector to 0,0
	 * */
	void Zero();
	/* Function: Length
	 * */
	float Length() const;
	/* Function: Length_squared
	 * */
	float Length_squared() const;
	/* Function: Normalize
	 * Does nothing if Length is 0.
	 */
	void Normalize();
	/* Function: Normalized
	 * Returns:
	 * Normalized vector or just itself if Length is 0.
	 */
	Vector Normalized() const;
	/* Function: Dotproduct
	 * */
	float Dotproduct(const Vector& v) const;
	/* Operator: operator+=
	 * */
	const Vector& operator+=(const Vector& v);
	/* Operator: operator-=
	 * */
	const Vector& operator-=(const Vector& v);
	/* Operator: operator*=
	 * */
	const Vector& operator*=(float v);
	/* Operator: operator/=
	 * */
	const Vector& operator/=(float v);
	/* Operator: operator+
	 * */
	const Vector operator+(const Vector& v) const;
	/* Operator: operator-
	 * */
	const Vector operator-(const Vector& v) const;
	/* Operator: operator-
	 * */
	const Vector operator-() const;
	/* Operator: operator/
	 * */
	Vector operator/(float s) const;
	/* Operator: operator*
	 * */
	Vector operator*(float s) const;
	/* Function: Angle
	 * */
	float Angle() const;
private:
	float x;
	float y;
};

#endif
