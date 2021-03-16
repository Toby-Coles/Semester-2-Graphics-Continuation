#include "ParticleModel.h"
ParticleModel::ParticleModel(Transform* transform)
{
	_transform = transform;
	//_position = *transform->GetPosition();
	_netForce._x = 0.0f; _netForce._y = 0.0f; _netForce._z = 0.0f;
	SetMass(2.0);
	_dampening = 0.98f;
	_velocity._x = 0.0;	_velocity._y = 0.0;	_velocity._z = 0.0;
	_accelleration._x = 0.0;  _accelleration._y = 0.0;  _accelleration._z = 0.0;
	_gravity = 9.81f;
	_vector = new Vector();
	_useGravity = true;
	

}

ParticleModel::~ParticleModel()
{
}

void ParticleModel::SetVelocity(float x, float y, float z)
{
	_velocity._x = x;
	_velocity._y = y;
	_velocity._z = z;
}

void ParticleModel::GetVelocity(Vector* velocity)
{
	*velocity = ParticleModel::_velocity;
}

void ParticleModel::SetMass(float mass)
{
	assert(mass != 0);
	_mass = mass;

	ParticleModel::_inverseMass = ((float)1.0) / _mass;
}





void ParticleModel::AddForce(const Vector& force)
{
	_forceAccumulator += force;
}

void ParticleModel::IntergrateMovement(float deltaTime)
{
	if (_inverseMass <= 0.0f) return;

	assert(deltaTime > 0.0);

	//Linear position
	//position.AddScaledVector(_velocity, deltaTime);
	

	//Accelleration
	Vector resultingAccelleration = _accelleration;
	resultingAccelleration.AddScaledVector(_forceAccumulator, _inverseMass);

	//Update linear velocity from the accelleration
	_velocity.AddScaledVector(resultingAccelleration, deltaTime);

	//Clear the forces so they do not effect the next intergration
	_forceAccumulator.Clear();

}

void ParticleModel::Move(float deltaTime)
{
	Vector previousPosition = *_transform->GetPosition();
	Vector newPosition;
	Vector previousVelocity = _velocity;

	//Update world position having added displacement to previous position
	//MAKE MORE EFFICIENT
	newPosition._x = previousPosition._x + previousVelocity._x * deltaTime + 0.5f * _accelleration._x * deltaTime * deltaTime;
	newPosition._y = previousPosition._y + previousVelocity._y * deltaTime + 0.5f * _accelleration._y * deltaTime * deltaTime;
	newPosition._z = previousPosition._z + previousVelocity._z * deltaTime + 0.5f * _accelleration._z * deltaTime * deltaTime;
	_transform->SetPosition(newPosition);
	
	_velocity._x = previousVelocity._x  + _accelleration._x * deltaTime;
	_velocity._y = previousVelocity._y  + _accelleration._y * deltaTime;
	_velocity._z = previousVelocity._z  + _accelleration._z * deltaTime;
	

}

void ParticleModel::UpdateAccelleration()
{
	_accelleration._x = _forceAccumulator._x / _mass;
	_accelleration._y = _forceAccumulator._y / _mass;
	_accelleration._z = _forceAccumulator._z / _mass;
}

void ParticleModel::Update(float deltaTime)
{
	IntergrateMovement(deltaTime);
	//UpdateNetForce();
	//UpdateAccelleration();
	Move(deltaTime);
	
}
