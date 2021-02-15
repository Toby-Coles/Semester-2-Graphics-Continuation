#include "Vector.h"
#include <cassert>
#include <tgmath.h>

Vector::Vector()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector::Vector(float x, float y, float z)
{
}

// ====== Addition ====== //
Vector Vector::operator+(const Vector& vec)
{
	return Vector(x + vec.x, y + vec.y, z + vec.z);
}

Vector& Vector::operator+=(const Vector& vec)
{
	x += vec.x;
	y += vec.y;
	z += vec.z;
	return *this;

}

// ====== Subtraction ====== //
Vector Vector::operator-(const Vector& vec)
{
	return Vector(x - vec.x, y - vec.y, z - vec.z);
}

Vector& Vector::operator-=(const Vector& vec)
{
	x -= vec.x;
	y -= vec.y;
	z -= vec.z;
	return *this;
}

Vector Vector::operator*(float value)
{
	return Vector(x * value, y * value, z * value);
}

Vector& Vector::operator*=(float value)
{
	x *= value;
	y *= value;
	z *= value;
	return *this;
}

Vector Vector::operator/(float value)
{
	assert(value != 0);
	return Vector(x / value, y / value, z / value);
}

Vector& Vector::operator/=(float value)
{
	assert(value != 0);
	x /= value;
	y /= value;
	z /= value;
	return *this;
}

//Vector& Vector::operator=(const Vector& vec)
//{
//	// TODO: insert return statement here
//}

float Vector::DotProduct(Vector& vec1, Vector& vec2)
{
	float result = ((vec1.x * vec2.x) + (vec1.y * vec2.y) + (vec1.z * vec2.z));
	return result;
}

float Vector::Magnitude(Vector& vec)
{

	float result = (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z);
	return sqrt(result);
}

Vector Vector::CrossProduct(Vector& vec1, Vector& vec2)
{
	Vector crossProduct;
	crossProduct.x = ((vec1.y * vec2.z) - (vec1.z * vec2.y));
	crossProduct.y = ((vec1.z * vec2.x) - (vec1.x * vec2.z));
	crossProduct.z = ((vec1.x * vec2.y) - (vec1.y * vec2.x));

	return crossProduct;
}

Vector Vector::Normalize(Vector& vec)
{
	float magnitudeOfVector = Magnitude(vec);
	Vector result = vec / magnitudeOfVector;

	return result;
}

float Vector::Square()
{
	return 0.0f;
}

float Vector::Distance(const Vector& vec)
{
	return 0.0f;
}



Vector::~Vector()
{
}