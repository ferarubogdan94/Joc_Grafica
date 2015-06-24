/** 
 *  author    Mihai Popescu
 *  date      03/15/2013
 *  copyright GNU Public License.
 */

#ifndef VEC2_H
#define VEC2_H

#define PI 3.14159265358979323846f

/**
 * Epsilon (the smallest float value used).
 */
#define EPS 1e-3 

/**
 * Helper class that handles 2D spatial vectors.
 */
struct Vec2
{
	Vec2();
	Vec2(float x, float y);

	Vec2& operator+=(const Vec2 &vector);
	Vec2& operator-=(const Vec2 &vector);
	Vec2& operator*=(float factor);
	Vec2& operator*=(const Vec2 &vector);
	Vec2& operator/=(float divisor);

	friend Vec2 operator-(const Vec2& v);

	friend bool operator==(const Vec2& v1, const Vec2& v2);
	friend bool operator!=(const Vec2& v1, const Vec2& v2);

	friend Vec2 operator+(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator-(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator*(const Vec2& v1, const Vec2& v2);
	friend Vec2 operator*(float factor, const Vec2& v);
	friend Vec2 operator*(const Vec2& v, float factor);
	friend Vec2 operator/(const Vec2& v, float divisor);

	friend float Dot(const Vec2& v1, const Vec2& v2);
	friend float Distance(const Vec2& v1, const Vec2& v2);
	friend float SquaredDistance(const Vec2& v1, const Vec2& v2);

	friend Vec2 Polar(float radius, float angle);

	Vec2& Rotate(float angle);
	float Length() const;
	float SquaredLength() const;
	float Normalize();
	float Argument() const;

public:
	float x, y;
};

#ifndef _DEBUG
#include "Vec2.inl"
#endif

#endif 
