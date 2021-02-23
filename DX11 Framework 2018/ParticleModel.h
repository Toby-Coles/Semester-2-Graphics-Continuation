#pragma once
#include "Vector.h"
#include "Transform.h"

class ParticleModel
{
public:
	ParticleModel(Transform* transform);
	~ParticleModel();

	

	void MoveConstVelocity( float deltaTime);

	void MoveConstAccelleration( float deltaTime);

	void SetVelocity(float x, float y, float z);
	Vector GetVelocity() const { return _velocity; }

	void SetMass(float mass);
	float GetMass() const { return _mass; }

	float GetNetForce() const { return _netForce; }


	void Update(float deltaTime);
	
protected:
	Transform* _transform;
	Vector _position;
	Vector _velocity;
	Vector _accelleration;
	float _deltaTime;

	float _mass;
	float _netForce;



	
private:
	Vector* _vector;
};



