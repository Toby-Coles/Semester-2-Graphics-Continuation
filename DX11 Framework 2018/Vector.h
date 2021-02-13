#pragma once

class Vector
{
public:

	float x, y, z;

	//Constructors
	Vector();
	Vector(float x, float y, float z);

	// ====== Mathematical Methods ====== //
	Vector operator+(const Vector& vec); // Vector Addition
	Vector& operator+=(const Vector& vec); // Assign result from addition

	Vector operator-(const Vector& vec); // Vector Subtraction
	Vector& operator-=(const Vector& vec); // Assign result from subtraction

	Vector operator*(float value); //Vector multiplication
	Vector& operator*=(float value); //Assign result from multiplication

	Vector operator/(float value); //Vector division
	Vector& operator/=(float value); //Assign result from division

	Vector& operator=(const Vector& vec);

	// ====== Vector Methods ====== //
	float DotProduct(Vector& vec1, Vector& vec2);

	Vector CrossProduct(Vector& vec1, Vector& vec2);
	Vector Normalize(Vector& vec);

	float Square(); // provides the square of the vector
	float Distance(const Vector& vec);
	float Magnitude(Vector& vec1);

	//// ====== Display Methods ====== //
	//float DisplayX();
	//float DisplayY();
	//float DisplayZ();

	~Vector();

private:

};

