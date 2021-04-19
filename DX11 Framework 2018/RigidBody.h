#pragma once
#include "Vector.h"
#include "Maths.h"
#include "Transform.h"

class RigidBody
{
public:
	RigidBody(Transform* transform);

	void CalculateDerivedData();
	void CalculateTransformMatrix(Matrix3x4& transform, const Vector& position, const Quaternion& orientation);

	void ClearAccums();
	void Intergrate(float deltaTime);

	void Move(float deltaTime);
	
	void AddForce(const Vector& forceToAdd);

	// ==== Set Inertia Tensors ==== //
	void SetCuboidInertiaTensor();

	//Adds a given force to a point on the rigid body, both in world space.
	void AddForceToPoint(const Vector& force, const Vector& point);

	//Adds a force to a given point, directino given in world coords, application in body space. 
	void AddForceToBodyPoint(const Vector& force, const Vector& point);

	void AddVelocity(Vector &deltaVelocity);
	void AddRotation(Vector& rotationChange);
	
	Vector GetWordSpacePoint(const Vector& point);
	void Update(float deltaTime);

	
	void SetMass(const float mass);
	void TransformInertiaTensor(Matrix3x3& invInertiaWorld, const Quaternion& q, const Matrix3x3 invertiBody, const Matrix3x4& rotMatrix);
	float GetMass()const { return _inverseMass; }

	float GetInverseMass() const;

	void SetInertiaTensor(const Matrix3x3& inertiaTensor);
	Matrix3x3 GetInertiaTensor() const { return _inverseInertiaTensor; }
	void GetInverseInertiaTensorWorld(Matrix3x3 *tensor);

	void SetPosition(Vector& position);
	Vector GetPosition() const { return _position; }
	void GetPosition(Vector *vector);
	void SetRotation(Vector& rotation);
	Vector GetRotation() const { return _rotation; }

	void SetAcceleration(const Vector accelleration);
	void SetAcceleration(const float x, float y, float z);
	void GetAcceleration(Vector* accelleration);

	Matrix3x4 GetTransform() const { return _transformMatrix; }
	Quaternion GetOrientation() const { return _orientation; }
	void SetOrientation(Quaternion orientation);

	void SetVelocity(const Vector& velocity);
	void SetVelocity(const float x, const float y, const float z);
	Vector GetVelocity()const { return _velocity; }

	Vector GetLastFrameAccelleration() const { return _lastFrameAccelleration; }

	void SetLinearDamping(const float damping);

	bool GetAwake() const { return isAwake; }
	void SetAwake(const bool awake);
	

	Transform* _transform;

protected:



	Vector _forceAccum;
	Vector _torqueAccum; 

	Vector _accelleration;
	Vector _lastFrameAccelleration;

	float _angularDamping;

	//Holds the inverse mass of the rigid body
	float _inverseMass;

	float motion;

	float _linearDamping;

	Vector _position;
	Quaternion _orientation;

	Vector _velocity;
	Vector _rotation;

	Matrix3x4 _transformMatrix;
	
	Matrix3x3 _inverseInertiaTensor;

	Matrix3x3 _inverseInertiaTensorWorld;

	bool isAwake;

	bool canSleep;


private:

};

