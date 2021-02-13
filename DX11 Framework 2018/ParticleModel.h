#pragma once
#include "Vector.h"


class ParticleModel
{
public:
	ParticleModel(float x, float y, float z);
	~ParticleModel();

	void MoveConstVelocity();

	void MoveConstAccelleration();

	Vector GetVelocity();
	Vector SetVelocity();
	void Update(float deltaTime);
	
protected:

	Vector _position;
	Vector _velocity;
	Vector _accelleration;
	float _deltaTime;

	
private:

};



