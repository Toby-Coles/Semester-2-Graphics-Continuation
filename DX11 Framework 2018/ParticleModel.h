#pragma once
#include "Vector.h"


class ParticleModel
{
public:
	ParticleModel(float x, float y, float z);
	~ParticleModel();

	void MoveConstVelocity(float deltaTime);

	Vector MoveConstAccelleration(Vector* position, float deltaTime);

	Vector GetVelocity() const { return _velocity; }

	void Update(float deltaTime);
	
protected:

	Vector _position;
	Vector _velocity;
	Vector _accelleration;
	float _deltaTime;

	
private:
	Vector* _vector;
};



