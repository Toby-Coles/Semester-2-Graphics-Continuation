#pragma once
#include "Vector.h"
#include "Transform.h"


	class ParticleModel
	{
	public:
		ParticleModel(Transform* transform);
		~ParticleModel();

		void SetVelocity(float x, float y, float z);
		Vector GetVelocity() const { return _velocity; }
		void GetVelocity(Vector* velocity);

		void SetMass(float mass);
		float GetMass() const { return _mass; }

		Vector GetNetForce() const { return _netForce; }


		
		void UpdateAccelleration();

		void AddForce(const Vector& force);

		void IntergrateMovement(float deltaTime);
		//Update world position and velocity from physics values
		void Move(float deltaTime);

		void Update(float deltaTime);

	protected:
		Transform* _transform;
		//Vector _position;
		Vector _velocity;
		Vector _accelleration;

		Vector _forceAccumulator;
		

		float _mass;
		float _inverseMass;
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





