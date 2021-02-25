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

	Vector GetNetForce() const { return _netForce; }


	void UpdateNetForce();
	void UpdateAccelleration();

	void AddThrust(Vector thrust);
	void AddBReaking(Vector breaking);
	void ApplyGravity();
	

	//Update world position and velocity from physics values
	void Move();

	void Update(float deltaTime);
	
protected:
	Transform* _transform;
	Vector _position;
	Vector _velocity;
	Vector _accelleration;
	float _deltaTime;

	float _mass;
	Vector _netForce;

	Vector _thrustForce;
	Vector _breakForce;
	Vector _friction;
	float _dampening;
	float _gravity;

	bool _useGravity;



	
private:
	Vector* _vector;
};



