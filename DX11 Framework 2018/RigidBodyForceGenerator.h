#pragma once
#include <vector>

#include "Maths.h"
#include "ParticleForceGenerator.h"
#include "RigidBody.h"


class RigidForceGen
{
public:
	/*RigidForceGen();
	~RigidForceGen();*/

	virtual void UpdateForce(RigidBody* body, float deltaTime) = 0;


private:

};

class RigidGravity : public RigidForceGen {

	Vector _gravity;

public:
	
	//Create Generator
	RigidGravity(const Vector &gravity);

	//Apply gravitational force
	virtual void UpdateForce(RigidBody *body, float deltaTime);

};

class RigidForceRegister
{

	struct ForceRegistration {
		RigidBody *body;
		RigidForceGen *fg;

	};
	
	typedef std::vector<ForceRegistration> Registry;
	Registry registrations;

public:

	void Add(RigidBody *body, RigidForceGen *fg);

	void Remove(RigidBody* body, RigidForceGen* fg);

	void Clear();

	void UpdateForces(float deltaTime);


private:

};


