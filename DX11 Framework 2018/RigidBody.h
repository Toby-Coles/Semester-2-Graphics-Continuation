#pragma once
#include "Vector.h"
#include "Maths.h"

class RigidBody
{
public:
	RigidBody();
	~RigidBody();

	void SetMass(const float mass);
	float GetMass()const { return _inverseMass; }

	void AddForce(const Vector& forceToAdd);

	//Adds a given force to a point on the rigid body, both in world space.
	void AddForceToPoint(const Vector& force, const Vector& point);

	//Adds a force to a given point, directino given in world coords, application in body space. 
	void AddForceToBodyPoint(const Vector& force, const Vector& point);

	void SetInertiaTensor(const Matrix3x3& inertiaTensor);

	Vector GetWordSpacePoint(const Vector& point);
	void Update(float deltaTime);

	void StartFrame();


protected:

	void CalculateDerivedData();
	void CalculateTransformMatrix(Matrix3x4& transform, const Vector& position, const Quaternion& orientation);

	


	void ClearAccums();
	void Intergrate(float deltaTime);

	
	
	Vector _forceAccum;
	Vector _torqueAccum; 

	Vector _accelleration;
	Vector _lastFrameAccelleration;

	float _angularDamping;

	//Holds the inverse mass of the rigid body
	float _inverseMass;

	float _linearDamping;

	Vector _position;
	Quaternion _orientation;

	Vector _velocity;
	Vector _rotation;

	Matrix3x4 _transformMatrix;
	
	Matrix3x3 _inverseInertiaTensor;

	Matrix3x3 _inverseInertiaTensorWorld;

private:

};

