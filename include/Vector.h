#ifndef Vector_h
#define Vector_h


class Vector
{
	public:
		Vector();
		Vector(float x, float y);

		const Vector &operator = (const Vector &v);

		bool operator == (const Vector &v) const;
		bool operator != (const Vector &v) const;

		void Set(float x, float y);

		float X() const;
		float Y() const;

		void Zero();
		float Length() const;
		float Length_squared() const;

		/* Normalize does nothing if Length is 0. */
		void Normalize();

		/* Normalized returns itself if Length is 0. */
		Vector Normalized() const;

		float Dotproduct(const Vector &v) const;
		const Vector &operator += (const Vector &v);
		const Vector &operator -= (const Vector &v);
		const Vector &operator *= (float v);
		const Vector &operator /= (float v);
		const Vector operator + (const Vector &v) const;
		const Vector operator - (const Vector &v) const;
		const Vector operator - () const;
		Vector operator / (float s) const;
		Vector operator * (float s) const;

		float Angle() const;

	private:
		float x;
		float y;
};


#endif  //  Vector_h
