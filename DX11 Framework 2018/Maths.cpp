#include "Maths.h"

float Matrix3x4::GetDeterminant() const
{
	return data[8] * data[5] * data[2] +
		data[4] * data[9] * data[2] +
		data[8] * data[1] * data[6] -
		data[0] * data[9] * data[6] -
		data[4] * data[1] * data[10] +
		data[0] * data[5] * data[10];
}

void Matrix3x4::SetInverse(const Matrix3x4& matrix)
{
	// Make sure the determinanterminant is non - zero.
		float determinant = GetDeterminant();
	if (determinant == 0) return;
	determinant = ((float)1.0f) / determinant;

	data[0] = (-matrix.data[9] * matrix.data[6] + matrix.data[5] * matrix.data[10]) * determinant;
	data[4] = (matrix.data[8] * matrix.data[6] - matrix.data[4] * matrix.data[10]) * determinant;
	data[8] = (-matrix.data[8] * matrix.data[5] + matrix.data[4] * matrix.data[9] * matrix.data[15]) * determinant;
	data[1] = (matrix.data[9] * matrix.data[2] - matrix.data[1] * matrix.data[10]) * determinant;
	data[5] = (-matrix.data[8] * matrix.data[2] + matrix.data[0] * matrix.data[10]) * determinant;
	data[9] = (matrix.data[8] * matrix.data[1] - matrix.data[0] * matrix.data[9] * matrix.data[15]) * determinant;
	data[2] = (-matrix.data[5] * matrix.data[2] + matrix.data[1] * matrix.data[6] * matrix.data[15]) * determinant; 
		data[6] = (+matrix.data[4] * matrix.data[2] - matrix.data[0] * matrix.data[6] * matrix.data[15]) * determinant;
	data[10] = (-matrix.data[4] * matrix.data[1] + matrix.data[0] * matrix.data[5] * matrix.data[15]) * determinant;


	data[3] = (matrix.data[9] * matrix.data[6] * matrix.data[3]
		- matrix.data[5] * matrix.data[10] * matrix.data[3]
		- matrix.data[9] * matrix.data[2] * matrix.data[7]
		+ matrix.data[1] * matrix.data[10] * matrix.data[7]
		+ matrix.data[5] * matrix.data[2] * matrix.data[11]
		- matrix.data[1] * matrix.data[6] * matrix.data[11]) * determinant;
	data[7] = (-matrix.data[8] * matrix.data[6] * matrix.data[3]
		+ matrix.data[4] * matrix.data[10] * matrix.data[3]
		+ matrix.data[8] * matrix.data[2] * matrix.data[7]
		- matrix.data[0] * matrix.data[10] * matrix.data[7]
		- matrix.data[4] * matrix.data[2] * matrix.data[11]
		+ matrix.data[0] * matrix.data[6] * matrix.data[11]) * determinant;
	data[11] = (matrix.data[8] * matrix.data[5] * matrix.data[3]
		- matrix.data[4] * matrix.data[9] * matrix.data[3]
		- matrix.data[8] * matrix.data[1] * matrix.data[7]
		+ matrix.data[0] * matrix.data[9] * matrix.data[7]
		+ matrix.data[4] * matrix.data[1] * matrix.data[11]
		- matrix.data[0] * matrix.data[5] * matrix.data[11]) * determinant;
}

void Matrix3x3::SetComponents(const Vector& cOne, const Vector& cTwo, const Vector& cThree)
{
	data[0] = cOne._x;
	data[1] = cTwo._x;
	data[2] = cThree._x;
	data[3] = cOne._y;
	data[4] = cTwo._y;
	data[5] = cThree._y;
	data[6] = cOne._z;
	data[7] = cTwo._z;
	data[8] = cThree._z;
}
