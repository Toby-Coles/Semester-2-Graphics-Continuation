#include "ParticleModel.h"
ParticleModel::ParticleModel(Transform* transform)
{
	_transform = transform;

	_velocity._x = 0.5;	_velocity._y = 0.5;	_velocity._z = 0.5;
	_accelleration._x = 0.5;  _accelleration._y = 0.5;  _accelleration._z = 0.5;

	_vector = new Vector();



}

ParticleModel::~ParticleModel()
{
}


void ParticleModel::MoveConstVelocity( float deltaTime)
{
	Vector previousPosition = *_transform->GetPosition();

	_transform->SetPosition( previousPosition + _velocity * deltaTime);

}

void ParticleModel::MoveConstAccelleration(float deltaTime) {
	Vector previousPosition = *_transform->GetPosition();
	Vector previousVelocity = _velocity;

	_transform->SetPosition(previousPosition + previousVelocity * deltaTime + _accelleration.operator*(0.5f) * deltaTime * deltaTime);
	_velocity = previousVelocity + _accelleration.operator*(deltaTime);

}

void ParticleModel::SetVelocity(float x, float y, float z)
{
	_velocity._x = x;
	_velocity._y = y;
	_velocity._z = z;
}

void ParticleModel::SetMass(float mass)
{
	_mass = mass;
}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;
}
