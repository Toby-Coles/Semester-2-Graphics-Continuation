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
	void operator-=( Vector& vec); // Assign result from subtraction

	Vector operator*(float value); //Vector multiplication
	void operator*=(float value); //Assign result from multiplication

	Vector operator/(float value); //Vector division
	void operator/=(float value); //Assign result from division

	Vector operator%(const Vector& vector);
	void operator %=(const Vector& vector);
	float operator* (const Vector &vector);

	float operator[](unsigned i) const;
	

	float& operator[](unsigned i);
	//Vector& operator=(const Vector& vec);

	// ====== Vector Methods ====== //
	float DotProduct(Vector& vec1, Vector& vec2);
	Vector VectorProduct(const Vector& vector);

	Vector CrossProduct(Vector& vec1, Vector& vec2);
	Vector Normalize(Vector& vec);

	float Square(); // provides the square of the vector
	float Distance(const Vector& vec);
	float Magnitude(Vector& vec1);
	float Magnitude();
	float SquareMagnitude();
	float ScalarProduct(Vector &vector);

	void AddScaledVector(const Vector& vector, float scale);
	void ComponentProductUpdate(const Vector& vector);

	void Invert();
	void Clear();

	//// ====== Display Methods ====== //
	//float DisplayX();
	//float DisplayY();
	//float DisplayZ();

	~Vector();

private:

};

