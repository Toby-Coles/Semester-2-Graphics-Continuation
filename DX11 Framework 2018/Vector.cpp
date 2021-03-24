#include "Vector.h"
#include <cassert>
#include <tgmath.h>

Vector::Vector()
{
	_x = 0.0f;
	_y = 0.0f;
	_z = 0.0f;
}

Vector::Vector(float x, float y, float z) {
	_x = x;
	_y = y;
	_z = z;


}


// ====== Addition ====== //
Vector Vector::operator+(const Vector& vec)
{
	return Vector(_x + vec._x, _y + vec._y, _z + vec._z);
}

void Vector::operator+=(const Vector& vec)
{
	_x += vec._x;
	_y += vec._y;
	_z += vec._z;
	

}

// ====== Subtraction ====== //
Vector Vector::operator-(const Vector& vec)
{
	return Vector(_x - vec._x, _y - vec._y, _z - vec._z);
}

void Vector::operator-=(const Vector& vec)
{
	_x -= vec._x;
	_y -= vec._y;
	_z -= vec._z;

}

// ====== Multiplication ====== //
Vector Vector::operator*(float value)
{
	return Vector(_x * value, _y * value, _z * value);
}

void Vector::operator*=(float value)
{
	_x *= value;
	_y *= value;
	_z *= value;

}

Vector Vector::operator/(float value)
{
	//assert(value != 0);
	return Vector(_x / value, _y / value, _z / value);
}

void Vector::operator/=(float value)
{
	assert(value != 0);
	_x /= value;
	_y /= value;
	_z /= value;
	
}

Vector Vector::operator%(const Vector& vector)
{
	return Vector(_y * vector._z - _z * vector._y,
		_z * vector._x - _x * vector._z,
		_x * vector._y - _y * vector._x);
}

void Vector::operator%=(const Vector& vector)
{
	*this = VectorProduct(vector);
}

//Vector& Vector::operator=(const Vector& vec)
//{
//	// TODO: insert return statement here
//}

float Vector::DotProduct(Vector& vec1, Vector& vec2)
{
	float result = ((vec1._x * vec2._x) + (vec1._y * vec2._y) + (vec1._z * vec2._z));
	return result;
}

Vector Vector::VectorProduct(const Vector& vector)
{
	return Vector(_y * vector._z - _z * vector._y,
		_z * vector._x - _x * vector._z,
		_x * vector._y - _y * vector._x);
}

float Vector::Magnitude(Vector& vec)
{

	float result = (vec._x * vec._x) + (vec._y * vec._y) + (vec._z * vec._z);
	return sqrt(result);
}

void Vector::AddScaledVector(const Vector& vector, float scale)
{
	_x += vector._x * scale;
	_y += vector._y * scale;
	_z += vector._z * scale;
}

void Vector::Invert()
{
	_x = -_x;
	_y = -_y;
	_z = -_z;
}

void Vector::Clear()
{
	_x = _y = _z = 0;
}

Vector Vector::CrossProduct(Vector& vec1, Vector& vec2)
{
	Vector crossProduct;
	crossProduct._x = ((vec1._y * vec2._z) - (vec1._z * vec2._y));
	crossProduct._y = ((vec1._z * vec2._x) - (vec1._x * vec2._z));
	crossProduct._z = ((vec1._x * vec2._y) - (vec1._y * vec2._x));

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