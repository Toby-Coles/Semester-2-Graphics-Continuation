#pragma once
#include "Vector.h"


class ParticleModel
{
public:
	ParticleModel();
	~ParticleModel();

	Vector MoveConstVelocity(Vector* position, float deltaTime);

	//Vector MoveConstAccelleration(Vector* position, float deltaTime);

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



