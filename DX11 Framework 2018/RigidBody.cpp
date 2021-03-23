#include "RigidBody.h"


void RigidBody::Intergrate(float deltaTime)
{
	//Calculate Linear accelleration from forces
	_lastFrameAccelleration = _accelleration;
	_lastFrameAccelleration.AddScaledVector(_forceAccum, _inverseMass);

	//Calculate angular Accelleration from torque inputs
	Vector angularAcceleration = _inverseInertiaTensorWorld.transform(_torqueAccum);

	//Adjust Velocity
	_velocity.AddScaledVector(_lastFrameAccelleration, deltaTime);

	//Update angular velocity
	_rotation.AddScaledVector(angularAcceleration, deltaTime);

	//Impose Drag
	_velocity *= pow(_linearDamping, deltaTime);
	_rotation *= pow(_angularDamping, deltaTime);

	//Update Linear Position
	_position.AddScaledVector(_velocity, deltaTime);

	//Update angular Position
	_orientation.addScaledVector(_rotation, deltaTime);

	CalculateDerivedData();


	ClearAccums();
}
void RigidBody::CalculateDerivedData()
{
	_orientation.normalise();
	CalculateTransformMatrix(_transformMatrix, _position, _orientation);

	//Calculate inertia tensor to world space
	_transformInertiaTensor(_inverseInertiaTensorWorld, _orientation, _inverseInertiaTensor, _transformMatrix);

}

void RigidBody::CalculateTransformMatrix(Matrix3x4& transform, const Vector& position, const Quaternion& orientation)
{

	_transformMatrix.data[0] = 1 - 2 * orientation.j * orientation.j -
		2 * orientation.k * orientation.k;
	_transformMatrix.data[1] = 2 * orientation.i * orientation.j -
		2 * orientation.r * orientation.k;
	_transformMatrix.data[2] = 2 * orientation.i * orientation.k +
		2 * orientation.r * orientation.j;
	_transformMatrix.data[3] = position._x;
	_transformMatrix.data[4] = 2 * orientation.i * orientation.j +
		2 * orientation.r * orientation.k;
	_transformMatrix.data[5] = 1 - 2 * orientation.i * orientation.i -
		2 * orientation.k * orientation.k;
	_transformMatrix.data[6] = 2 * orientation.j * orientation.k -
		2 * orientation.r * orientation.i;
	_transformMatrix.data[7] = position._y;
	_transformMatrix.data[8] = 2 * orientation.i * orientation.k -
		2 * orientation.r * orientation.j;
	_transformMatrix.data[9] = 2 * orientation.j * orientation.k +
		2 * orientation.r * orientation.i;
	_transformMatrix.data[10] = 1 - 2 * orientation.i * orientation.i -
		2 * orientation.j * orientation.j;
	_transformMatrix.data[11] = position._z;
}

void RigidBody::SetInertiaTensor(const Matrix3x3& inertiaTensor)
{
	_inverseInertiaTensor.SetInverse(inertiaTensor);
}

void RigidBody::AddForceToPoint(const Vector& force, const Vector& point)
{
	Vector pt = point;
	pt -= _position;

	_forceAccum += force;
	_torqueAccum += pt % force;
}

void RigidBody::AddForceToBodyPoint(const Vector& force, const Vector& point)
{
	//Convert to be relative to centre of mass
	Vector pt = GetWordSpacePoint(point);
	AddForceToPoint(force, pt);

}

Vector RigidBody::GetWordSpacePoint(const Vector& point)
{
	return _transformMatrix.transform(point);
}

void RigidBody::Update(float deltaTime)
{
	Intergrate(deltaTime);
}

void RigidBody::AddForce(const Vector& forceToAdd)
{
	_forceAccum += forceToAdd;

}

void RigidBody::ClearAccums()
{
	_forceAccum.Clear();
	_torqueAccum.Clear();
}



void RigidBody::SetMass(const float mass)
{
	assert(mass != 0);
	RigidBody::_inverseMass = ((float)1.0) / mass;
}

static inline void _transformInertiaTensor(Matrix3x3& invInertiaWorld, const Quaternion& q, const Matrix3x3 invertiBody, const Matrix3x4 &rotMatrix) {
	
	float t4 = rotMatrix.data[0] * invertiBody.data[0] +
		rotMatrix.data[1] * invertiBody.data[3] +
		rotMatrix.data[2] * invertiBody.data[6];
	float t9 = rotMatrix.data[0] * invertiBody.data[1] +
		rotMatrix.data[1] * invertiBody.data[4] +
		rotMatrix.data[2] * invertiBody.data[7];
	float t14 = rotMatrix.data[0] * invertiBody.data[2] +
		rotMatrix.data[1] * invertiBody.data[5] +
		rotMatrix.data[2] * invertiBody.data[8];
	float t28 = rotMatrix.data[4] * invertiBody.data[0] +
		rotMatrix.data[5] * invertiBody.data[3] +
		rotMatrix.data[6] * invertiBody.data[6];
	float t33 = rotMatrix.data[4] * invertiBody.data[1] +
		rotMatrix.data[5] * invertiBody.data[4] +
		rotMatrix.data[6] * invertiBody.data[7];
	float t38 = rotMatrix.data[4] * invertiBody.data[2] +
		rotMatrix.data[5] * invertiBody.data[5] +
		rotMatrix.data[6] * invertiBody.data[8];
	float t52 = rotMatrix.data[8] * invertiBody.data[0] +
		rotMatrix.data[9] * invertiBody.data[3] +
		rotMatrix.data[10] * invertiBody.data[6];
	float t57 = rotMatrix.data[8] * invertiBody.data[1] +
		rotMatrix.data[9] * invertiBody.data[4] +
		rotMatrix.data[10] * invertiBody.data[7];
	float t62 = rotMatrix.data[8] * invertiBody.data[2] +
		rotMatrix.data[9] * invertiBody.data[5] +
		rotMatrix.data[10] * invertiBody.data[8];
	invInertiaWorld.data[0] = t4 * rotMatrix.data[0] +
		t9 * rotMatrix.data[1] +
		t14 * rotMatrix.data[2];

	invInertiaWorld.data[1] = t4 * rotMatrix.data[4] +
		t9 * rotMatrix.data[5] +
		t14 * rotMatrix.data[6];
	invInertiaWorld.data[2] = t4 * rotMatrix.data[8] +
		t9 * rotMatrix.data[9] +
		t14 * rotMatrix.data[10];
	invInertiaWorld.data[3] = t28 * rotMatrix.data[0] +
		t33 * rotMatrix.data[1] + 
		t38 * rotMatrix.data[2];
	invInertiaWorld.data[4] = t28 * rotMatrix.data[4] +
		t33 * rotMatrix.data[5] +
		t38 * rotMatrix.data[6];
	invInertiaWorld.data[5] = t28 * rotMatrix.data[8] +
		t33 * rotMatrix.data[9] +
		t38 * rotMatrix.data[10];
	invInertiaWorld.data[6] = t52 * rotMatrix.data[0] +
		t57 * rotMatrix.data[1] +
		t62 * rotMatrix.data[2];
	invInertiaWorld.data[7] = t52 * rotMatrix.data[4] +
		t57 * rotMatrix.data[5] +
		t62 * rotMatrix.data[6];
	invInertiaWorld.data[8] = t52 * rotMatrix.data[8] +
		t57 * rotMatrix.data[9] +
		t62 * rotMatrix.data[10];


}
