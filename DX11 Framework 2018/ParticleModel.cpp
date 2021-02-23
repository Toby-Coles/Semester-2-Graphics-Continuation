#include "ParticleModel.h"
ParticleModel::ParticleModel()
{
	
	_velocity._x = 0.5;	_velocity._y = 0.5;	_velocity._z = 0.5;
	_accelleration._x = 0.01;  _accelleration._y = 0.01;  _accelleration._z = 0.01;

	_vector = new Vector();



}

ParticleModel::~ParticleModel()
{
}

Vector ParticleModel::MoveConstVelocity(Vector* position, float deltaTime)
{
	Vector previousPosition = *position;

	return previousPosition + _velocity * deltaTime;

}

//Vector ParticleModel::MoveConstAccelleration(Vector* position, float deltaTime) {
//	Vector* previousPosition = position;
//	Vector* previousVelocity = &_velocity;
//
//	_vector = &_accelleration;
//
//	Vector* result = new Vector();
//	//result = previousPosition->operator+(previousVelocity->operator*(deltaTime)) + _vector->operator*(0.5f) * deltaTime * deltaTime;
//	//return result;
//	return result;
//
//	
//	
//}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;
}
