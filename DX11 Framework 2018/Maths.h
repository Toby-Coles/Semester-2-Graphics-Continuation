#pragma once

#include <float.h>
#include <math.h>
#include <directxmath.h>
#include <d3d11_1.h>
#include "Vector.h"

using namespace DirectX;

// REMOVE ME! 
// Vector3 here should be removed, use your own Vector class instead throughout this class


class Quaternion
{
public:
	/**
	* Holds the float component of the quaternion.
	*/
	float r;

	/**
	* Holds the first complex component of the
	* quaternion.
	*/
	float i;

	/**
	* Holds the second complex component of the
	* quaternion.
	*/
	float j;

	/**
	* Holds the third complex component of the
	* quaternion.
	*/
	float k;

	/**
	* The default constructor creates a quaternion representing
	* a zero rotation.
	*/
	float data[4];
	Quaternion() : r(1), i(0), j(0), k(0) {}

	/**
	* The explicit constructor creates a quaternion with the given
	* components.
	*
	* @param r The float component of the rigid body's orientation
	* quaternion.
	*
	* @param i The first complex component of the rigid body's
	* orientation quaternion.
	*
	* @param j The second complex component of the rigid body's
	* orientation quaternion.
	*
	* @param k The third complex component of the rigid body's
	* orientation quaternion.
	*
	* @note The given orientation does not need to be normalised,
	* and can be zero. This function will not alter the given
	* values, or normalise the quaternion. To normalise the
	* quaternion (and make a zero quaternion a legal rotation),
	* use the normalise function.
	*
	* @see normalise
	*/
	Quaternion(const float r, const float i, const float j, const float k)
		: r(r), i(i), j(j), k(k)
	{
	}

	/**
	* Normalises the quaternion to unit length, making it a valid
	* orientation quaternion.
	*/
	void normalise()
	{
		float d = r * r + i * i + j * j + k * k;

		// Check for zero length quaternion, and use the no-rotation
		// quaternion in that case.
		if (d < FLT_EPSILON)
		{
			r = 1;
			return;
		}

		d = static_cast<float>(1.0) / sqrtf(d);
		r *= d;
		i *= d;
		j *= d;
		k *= d;
	}

	/**
	* Multiplies the quaternion by the given quaternion.
	*
	* @param multiplier The quaternion by which to multiply.
	*/
	void operator *=(const Quaternion& multiplier)
	{
		Quaternion q = *this;
		r = q.r * multiplier.r - q.i * multiplier.i -
			q.j * multiplier.j - q.k * multiplier.k;
		i = q.r * multiplier.i + q.i * multiplier.r +
			q.j * multiplier.k - q.k * multiplier.j;
		j = q.r * multiplier.j + q.j * multiplier.r +
			q.k * multiplier.i - q.i * multiplier.k;
		k = q.r * multiplier.k + q.k * multiplier.r +
			q.i * multiplier.j - q.j * multiplier.i;
	}

	/**
	* Adds the given vector to this, scaled by the given amount.
	* This is used to update the orientation quaternion by a rotation
	* and time.
	*
	* @param vector The vector to add.
	*
	* @param scale The amount of the vector to add.
	*/
	void addScaledVector(const Vector& vector, float scale)
	{
		Quaternion q(0,
			vector._x * scale,
			vector._y * scale,
			vector._z * scale);
		q *= *this;
		r += q.r * 0.5f;
		i += q.i * 0.5f;
		j += q.j * 0.5f;
		k += q.k * 0.5f;
	}

	void rotateByVector(const Vector& vector)
	{
		Quaternion q(0, vector._x, vector._y, vector._z);
		(*this) *= q;
	}
};

/**
* Inline function that creates a   matrix from a
* position and orientation.
*/
static inline void CalculateTransformMatrixColumnMajor(XMMATRIX& transformMatrix,
	const Vector& position,
	const Quaternion& orientation)
{
	transformMatrix.r[0] = XMVectorSetX(transformMatrix.r[0], 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k);
	transformMatrix.r[0] = XMVectorSetY(transformMatrix.r[0], 2 * orientation.i * orientation.j -
		2 * orientation.r * orientation.k);
	transformMatrix.r[0] = XMVectorSetZ(transformMatrix.r[0], 2 * orientation.i * orientation.k +
		2 * orientation.r * orientation.j);
	transformMatrix.r[0] = XMVectorSetW(transformMatrix.r[0], 0.0f);

	transformMatrix.r[1] = XMVectorSetX(transformMatrix.r[1], 2 * orientation.i * orientation.j +
		2 * orientation.r * orientation.k);
	transformMatrix.r[1] = XMVectorSetY(transformMatrix.r[1], 1 - 2 * orientation.i * orientation.i -
		2 * orientation.k * orientation.k);
	transformMatrix.r[1] = XMVectorSetZ(transformMatrix.r[1], 2 * orientation.j * orientation.k -
		2 * orientation.r * orientation.i);
	transformMatrix.r[1] = XMVectorSetW(transformMatrix.r[1], 0.0f);

	transformMatrix.r[2] = XMVectorSetX(transformMatrix.r[2], 2 * orientation.i * orientation.k -
		2 * orientation.r * orientation.j);
	transformMatrix.r[2] = XMVectorSetY(transformMatrix.r[2], 2 * orientation.j * orientation.k +
		2 * orientation.r * orientation.i);
	transformMatrix.r[2] = XMVectorSetZ(transformMatrix.r[2], 1 - 2 * orientation.i * orientation.i -
		2 * orientation.j * orientation.j);
	transformMatrix.r[2] = XMVectorSetW(transformMatrix.r[2], 0.0f);

	transformMatrix.r[3] = XMVectorSetX(transformMatrix.r[3], position._x);
	transformMatrix.r[3] = XMVectorSetY(transformMatrix.r[3], position._y);
	transformMatrix.r[3] = XMVectorSetZ(transformMatrix.r[3], position._z);
	transformMatrix.r[3] = XMVectorSetW(transformMatrix.r[3], 1.0f);
}

static inline void CalculateTransformMatrixRowMajor(XMMATRIX& transformMatrix,
	const Vector& position,
	const Quaternion& orientation)
{
	transformMatrix.r[0] = XMVectorSetX(transformMatrix.r[0], 1 - 2 * orientation.j * orientation.j - 2 * orientation.k * orientation.k);
	transformMatrix.r[0] = XMVectorSetY(transformMatrix.r[0], 2 * orientation.i * orientation.j - 2 * orientation.r * orientation.k);
	transformMatrix.r[0] = XMVectorSetZ(transformMatrix.r[0], 2 * orientation.i * orientation.k + 2 * orientation.r * orientation.j);
	transformMatrix.r[0] = XMVectorSetW(transformMatrix.r[0], position._x);

	transformMatrix.r[1] = XMVectorSetX(transformMatrix.r[1], 2 * orientation.i * orientation.j + 2 * orientation.r * orientation.k);
	transformMatrix.r[1] = XMVectorSetY(transformMatrix.r[1], 1 - 2 * orientation.i * orientation.i - 2 * orientation.k * orientation.k);
	transformMatrix.r[1] = XMVectorSetZ(transformMatrix.r[1], 2 * orientation.j * orientation.k - 2 * orientation.r * orientation.i);
	transformMatrix.r[1] = XMVectorSetW(transformMatrix.r[1], position._y);

	transformMatrix.r[2] = XMVectorSetX(transformMatrix.r[2], 2 * orientation.i * orientation.k - 2 * orientation.r * orientation.j);
	transformMatrix.r[2] = XMVectorSetY(transformMatrix.r[2], 2 * orientation.j * orientation.k + 2 * orientation.r * orientation.i);
	transformMatrix.r[2] = XMVectorSetZ(transformMatrix.r[2], 1 - 2 * orientation.i * orientation.i - 2 * orientation.j * orientation.j);
	transformMatrix.r[2] = XMVectorSetW(transformMatrix.r[2], position._z);

	/*
	transformMatrix.r[3] = XMVectorSetX(transformMatrix.r[3], position.x);
	transformMatrix.r[3] = XMVectorSetY(transformMatrix.r[3], position.y);
	transformMatrix.r[3] = XMVectorSetZ(transformMatrix.r[3], position.z);
	*/

	transformMatrix.r[3] = XMVectorSetX(transformMatrix.r[3], 0);
	transformMatrix.r[3] = XMVectorSetY(transformMatrix.r[3], 0);
	transformMatrix.r[3] = XMVectorSetZ(transformMatrix.r[3], 0);
	transformMatrix.r[3] = XMVectorSetW(transformMatrix.r[3], 1.0f);

	transformMatrix = XMMatrixTranspose(transformMatrix);

	transformMatrix = transformMatrix;
}

// ===================== Matrix Data Structures ===================== // 
class Matrix3x3
{
public:
	Matrix3x3() {
	
		data[0] = data[1] = data[2] = data[3] = data[4] = data[5] =
			data[6] = data[7] = data[8] = 0;
	};

	

	Matrix3x3(float c0, float c1, float c2, float c3, float c4, float c5, float c6, float c7, float c8) {
		data[0] = c0; data[1] = c1; data[2] = c2;
		data[3] = c3; data[4] = c4; data[5] = c5;
		data[6] = c6; data[7] = c7; data[8] = c8;
	}

	

	//Sets the matrix to invert based off the given matrix
	//void SetInverse(const Matrix3x3& m) {

	//	float t1 = m.data[0] * m.data[4];
	//	float t2 = m.data[0] * m.data[5];
	//	float t3 = m.data[1] * m.data[3];
	//	float t4 = m.data[2] * m.data[3];
	//	float t5 = m.data[1] * m.data[6];
	//	float t6 = m.data[2] * m.data[6];

	//	// Calculate the determinant.
	//	float determinant = (t1 * m.data[8] - t2 * m.data[7] - t3 * m.data[8] +
	//		t4 * m.data[7] + t5 * m.data[5] - t6 * m.data[4]);

	//	// Make sure the determinant is non-zero.
	//	if (determinant == (float)0.0f) return;
	//	float invd = (float)1.0f / determinant;

	//	data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * invd;
	//	data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * invd;
	//	data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * invd;
	//	data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * invd;
	//	data[4] = (m.data[0] * m.data[8] - t6) * invd;
	//	data[5] = -(t2 - t4) * invd;
	//	data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * invd;
	//	data[7] = -(m.data[0] * m.data[7] - t5) * invd;
	//	data[8] = (t1 - t3) * invd;
	//}


	void SetInverse(const Matrix3x3& m) {

		float t4 = m.data[0] * m.data[4];
		float t6 = m.data[0] * m.data[5];
		float t8 = m.data[1] * m.data[3];
		float t10 = m.data[2] * m.data[3];
		float t12 = m.data[1] * m.data[6];
		float t14 = m.data[2] * m.data[6];

		// Calculate the determinant
		float t16 = (t4 * m.data[8] - t6 * m.data[7] - t8 * m.data[8] +
			t10 * m.data[7] + t12 * m.data[5] - t14 * m.data[4]);

		// Make sure the determinant is non-zero.
		if (t16 == (float)0.0f) return;
		float t17 = 1 / t16;

		data[0] = (m.data[4] * m.data[8] - m.data[5] * m.data[7]) * t17;
		data[1] = -(m.data[1] * m.data[8] - m.data[2] * m.data[7]) * t17;
		data[2] = (m.data[1] * m.data[5] - m.data[2] * m.data[4]) * t17;
		data[3] = -(m.data[3] * m.data[8] - m.data[5] * m.data[6]) * t17;
		data[4] = (m.data[0] * m.data[8] - t14) * t17;
		data[5] = -(t6 - t10) * t17;
		data[6] = (m.data[3] * m.data[7] - m.data[4] * m.data[6]) * t17;
		data[7] = -(m.data[0] * m.data[7] - t12) * t17;
		data[8] = (t4 - t8) * t17;
	}

	void SetCuboidInertiaTensor( Vector& halfSize, float mass) {
		Vector squares = halfSize.ComponentProduct(halfSize);
		SetInertiaTensorCoefficients(0.3f * mass * (squares._y + squares._z), 0.3f * mass * (squares._x + squares._z),
			0.3f * mass * squares._x + squares._y);
		
	}

	void SetInertiaTensorCoefficients(float ix, float iy, float iz, float ixy = 0, float ixz = 0, float iyz = 0)
	{
		data[0] = ix;
		data[1] = data[3] = -ixy;
		data[2] = data[6] = -ixz;
		data[4] = iy;
		data[5] = data[7] = -iyz;
		data[8] = iz;

	}

	Matrix3x3 Inverse()const {

		Matrix3x3 result;
		result.SetInverse(*this);
		return result;
	}

	//Performs the invert
	void Invert() {
		SetInverse(*this);
	}

	//Sets the matrix to be rotatino matrix to the passed in quarternion 
	void SetOrientation(const Quaternion& q)
	{

		data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
		data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
		data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
		data[3] = 2 * q.i * q.j - 2 * q.k * q.r;
		data[4] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
		data[5] = 2 * q.j * q.k + 2 * q.i * q.r;
		data[6] = 2 * q.i * q.k + 2 * q.j * q.r;
		data[7] = 2 * q.j * q.k - 2 * q.i * q.r;
		data[8] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
	}

	void SetSkewSymmetric(const Vector vector) {
		data[0] = data[4] = data[8] = 0;
		data[1] = -vector._z;
		data[2] = vector._y;
		data[3] = vector._z;
		data[5] = -vector._x;
		data[6] = -vector._y;
		data[7] = vector._x;
	}

	// Matrix3 Multiplier overload (Transform the given vector by matrix )
	Vector operator*(const Vector& vector)const {

		return Vector(
			vector._x * data[0] + vector._y * data[1] + vector._z * data[2],
			vector._x * data[3] + vector._y * data[4] + vector._z * data[5],
			vector._x * data[6] + vector._y * data[7] + vector._z * data[8]
		);
	}

	Matrix3x3 operator*(const Matrix3x3 &o)const 
	{
		return Matrix3x3(
			data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6],
			data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7],
			data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8],

			data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6],
			data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7],
			data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8],

			data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6],
			data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7],
			data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8]
		);
	}

	void operator*=(const Matrix3x3 &o) {
		float t1;
		float t2;
		float t3;

		t1 = data[0] * o.data[0] + data[1] * o.data[3] + data[2] * o.data[6];
		t2 = data[0] * o.data[1] + data[1] * o.data[4] + data[2] * o.data[7];
		t3 = data[0] * o.data[2] + data[1] * o.data[5] + data[2] * o.data[8];
		data[0] = t1;
		data[1] = t2;
		data[2] = t3;

		t1 = data[3] * o.data[0] + data[4] * o.data[3] + data[5] * o.data[6];
		t2 = data[3] * o.data[1] + data[4] * o.data[4] + data[5] * o.data[7];
		t3 = data[3] * o.data[2] + data[4] * o.data[5] + data[5] * o.data[8];
		data[3] = t1;
		data[4] = t2;
		data[5] = t3;

		t1 = data[6] * o.data[0] + data[7] * o.data[3] + data[8] * o.data[6];
		t2 = data[6] * o.data[1] + data[7] * o.data[4] + data[8] * o.data[7];
		t3 = data[6] * o.data[2] + data[7] * o.data[5] + data[8] * o.data[8];
		data[6] = t1;
		data[7] = t2;
		data[8] = t3;
	}

	//Multiply by a given scalar
	void operator*=(const float scalar)
	{
		data[0] *= scalar; data[1] *= scalar; data[2] *= scalar;
		data[3] *= scalar; data[4] *= scalar; data[5] *= scalar;
		data[6] *= scalar; data[7] *= scalar; data[8] *= scalar;
	}

	//Component-wise addition of given matrices
	void operator+=(const Matrix3x3& o) {
		data[0] += o.data[0]; data[1] += o.data[1]; data[2] += o.data[2];
		data[3] += o.data[3]; data[4] += o.data[4]; data[5] += o.data[5];
		data[6] += o.data[6]; data[7] += o.data[7]; data[8] += o.data[8];
	}

	
	Vector transform(const Vector &vector) const
	{
		return (*this) * vector;
	}

	//Sets transpose matrix
	void SetTranspose(const Matrix3x3& matrix) {

		data[0] = matrix.data[0];
		data[1] = matrix.data[3];
		data[2] = matrix.data[6];
		data[3] = matrix.data[1];
		data[4] = matrix.data[4];
		data[5] = matrix.data[7];
		data[6] = matrix.data[2];
		data[7] = matrix.data[5];
		data[8] = matrix.data[8];
	}

	Matrix3x3 Transpose()const {
		Matrix3x3 result;
		result.SetTranspose(*this);
		return result;
	}

	Vector TransformTranspose(const Vector& vector) const {
		return Vector(
			vector._x * data[0] + vector._y * data[3] + vector._z * data[6],
			vector._x * data[1] + vector._y * data[4] + vector._z * data[7],
			vector._x * data[2] + vector._y * data[5] + vector._z * data[8]
		);
	}

	void SetComponents(const Vector &cOne, const Vector &cTwo, const Vector &cThree);
	


	float data[9];

private:

};

class Matrix3x4
{
public:
	Matrix3x4()
	{
		data[1] = data[2] = data[3] = data[4] = data[6] =
			data[7] = data[8] = data[9] = data[11] = 0;
		data[0] = data[5] = data[10] = 1;
	}

	

	//Return determinant
	float GetDeterminant() const;

	void SetInverse(const Matrix3x4& matrix);

	//Set matrix for invert
	Matrix3x4 Inverse()const {

		Matrix3x4 result;
		result.SetInverse(*this);
		return result;
	}

	void Invert() {
		SetInverse(*this);
	}

	////Transforms the given vector by the matrix
	//Vector transform(const Vector& vector)const {
	//	return (*this) * vector;

	//}

	void SetOrientationAndPosition(const Quaternion& q, const Vector& pos) {

		data[0] = 1 - (2 * q.j * q.j + 2 * q.k * q.k);
		data[1] = 2 * q.i * q.j + 2 * q.k * q.r;
		data[2] = 2 * q.i * q.k - 2 * q.j * q.r;
		data[3] = pos._x;

		data[4] = 2 * q.i * q.j - 2 * q.k * q.r;
		data[5] = 1 - (2 * q.i * q.i + 2 * q.k * q.k);
		data[6] = 2 * q.j * q.k + 2 * q.i * q.r;
		data[7] = pos._y;

		data[8] = 2 * q.i * q.k + 2 * q.j * q.r; 
		data[9] = 2 * q.j * q.k - 2 * q.i * q.r;
		data[10] = 1 - (2 * q.i * q.i + 2 * q.j * q.j);
		data[11] = pos._z;
	}

	Vector TransformInverse(const Vector& vector)const {

		Vector temp = vector;
		temp._x -= data[3];
		temp._y -= data[7];
		temp._z -= data[11];

		return Vector(
			temp._x * data[0] +
			temp._y * data[4] +
			temp._z * data[8],
			temp._x * data[1] +
			temp._y * data[5] +
			temp._z * data[9],
			temp._x * data[2] +
			temp._y * data[6] +
			temp._z * data[10]);
	}

	Vector ConvWorldToLocal(const Vector& world, const Matrix3x4 transform) {
		return transform.TransformInverse(world);
	}

	//Gets a vector representing one axis in the matrix
	Vector GetAxisVector(int i)const  {
		return Vector(data[i], data[i + 4], data[i + 8]);
	}
	Vector TransformDirection(const Vector& vector) const
	{
		return Vector(
			vector._x * data[0] +
			vector._y * data[1] +
			vector._z * data[2],
			vector._x * data[4] +
			vector._y * data[5] +
			vector._z * data[6],
			vector._x * data[8] +
			vector._y * data[9] +
			vector._z * data[10]
		);
	}

	Vector TransformInverseDirection(const Vector& vector) const
	{
		return Vector(
			vector._x * data[0] +
			vector._y * data[4] +
			vector._z * data[8],
			vector._x * data[1] +
			vector._y * data[5] +
			vector._z * data[9],
			vector._x * data[2] +
			vector._y * data[6] +
			vector._z * data[10]
		);
	}

	//Apply either local to world, or world to  local conversions(DIRECTION VECTOR)

	Vector ConvertLocalToWorldDirection(const Vector& local, const Matrix3x4& transform) {
		return transform.TransformDirection(local);
	}
	Vector ConvertWorldToLocalDirection(const Vector& world, const Matrix3x4& transform) {
		return transform.TransformInverseDirection(world);
	}


	// Matrix4 Multiplier overload (Transform the given vector by matrix )
	Vector operator*(const Vector& vector) const
	{
		return Vector(
			vector._x * data[0] +
			vector._y * data[1] +
			vector._z * data[2] + data[3],
			vector._x * data[4] +
			vector._y * data[5] +
			vector._z * data[6] + data[7],
			vector._x * data[8] +
			vector._y * data[9] +
			vector._z * data[10] + data[11]
		);
	}

	
	Vector transform(const Vector& vector) const
	{
		return (*this) * vector;
	}

	Matrix3x4 operator*(const Matrix3x4& o)const
	{
		Matrix3x4 result;
		result.data[0] = (o.data[0] * data[0]) + (o.data[4] * data[1]) + (o.data[8] * data[2]);
		result.data[4] = (o.data[0] * data[4]) + (o.data[4] * data[5]) + (o.data[8] * data[6]);
		result.data[8] = (o.data[0] * data[8]) + (o.data[4] * data[9]) + (o.data[8] * data[10]);

		result.data[1] = (o.data[1] * data[0]) + (o.data[5] * data[1]) + (o.data[9] * data[2]);
		result.data[5] = (o.data[1] * data[4]) + (o.data[5] * data[5]) + (o.data[9] * data[6]);
		result.data[9] = (o.data[1] * data[8]) + (o.data[5] * data[9]) + (o.data[9] * data[10]);

		result.data[2] = (o.data[2] * data[0]) + (o.data[6] * data[1]) + (o.data[10] * data[2]);
		result.data[6] = (o.data[2] * data[4]) + (o.data[6] * data[5]) + (o.data[10] * data[6]);
		result.data[10] = (o.data[2] * data[8]) + (o.data[6] * data[9]) + (o.data[10] * data[10]);

		result.data[3] = (o.data[3] * data[0]) + (o.data[7] * data[1]) + (o.data[11] * data[2]) + data[3];
		result.data[7] = (o.data[3] * data[4]) + (o.data[7] * data[5]) + (o.data[11] * data[6]) + data[7];
		result.data[11] = (o.data[3] * data[8]) + (o.data[7] * data[9]) + (o.data[11] * data[10]) + data[11];

		return result;
	}

	float data[12];

	
private:

};

