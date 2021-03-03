#pragma once

class Vector
{
public:

	float _x, _y, _z;

	//Constructors
	Vector();
	Vector(float x, float y, float z);


	// ====== Mathematical Methods ====== //
	Vector operator+(const Vector& vec); // Vector Addition
	void operator+=(const Vector& vec); // Assign result from addition

	Vector operator-(const Vector& vec); // Vector Subtraction
	void operator-=(const Vector& vec); // Assign result from subtraction

	Vector operator*(float value); //Vector multiplication
	void operator*=(float value); //Assign result from multiplication

	Vector operator/(float value); //Vector division
	void operator/=(float value); //Assign result from division

	//Vector& operator=(const Vector& vec);

	// ====== Vector Methods ====== //
	float DotProduct(Vector& vec1, Vector& vec2);

	Vector CrossProduct(Vector& vec1, Vector& vec2);
	Vector Normalize(Vector& vec);

	float Square(); // provides the square of the vector
	float Distance(const Vector& vec);
	float Magnitude(Vector& vec1);

	void AddScaledVector(const Vector& vector, float scale);

	void Invert();
	void Clear();

	//// ====== Display Methods ====== //
	//float DisplayX();
	//float DisplayY();
	//float DisplayZ();

	~Vector();

private:

};

