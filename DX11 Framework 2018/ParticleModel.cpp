#include "ParticleModel.h"
ParticleModel::ParticleModel(float x, float y, float z)
{
	_position.x = x;
	_position.y = y;
	_position.z = z;

	_velocity.x = 0.5;	_velocity.y = 0.5;	_velocity.z = 0.5;
	_accelleration.x = 0.5;  _accelleration.y = 0.5;  _accelleration.z = 0.5;




}

ParticleModel::~ParticleModel()
{
}

void ParticleModel::MoveConstVelocity()
{
	Vector previousPosition = _position;
	_position = previousPosition + _velocity * _deltaTime;

}
void ParticleModel::MoveConstAccelleration() {
	Vector previousPosition = _position;
	Vector previousVelocity = _velocity;


	_position = previousPosition + previousVelocity * _deltaTime + 0.5 * _accelleration

}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;
}
