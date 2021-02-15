#include "ParticleModel.h"
ParticleModel::ParticleModel(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_velocity.x = 0.5;	_velocity.y = 0.5;	_velocity.z = 0.5;
	_accelleration.x = 0.5;  _accelleration.y = 0.5;  _accelleration.z = 0.5;

	_vector = new Vector();



}

ParticleModel::~ParticleModel()
{
}

void ParticleModel::MoveConstVelocity(float deltaTime)
{
	Vector previousPosition = _position;
	_position = previousPosition + _velocity * deltaTime;

}
Vector ParticleModel::MoveConstAccelleration(Vector* position, float deltaTime) {
	Vector* previousPosition = position;
	Vector* previousVelocity = &_velocity;

	_vector = &_accelleration;

	return previousPosition->operator+(previousVelocity->operator*(deltaTime)) + _vector->operator*(0.5f) * deltaTime * deltaTime;
	

	
	
}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;
}
