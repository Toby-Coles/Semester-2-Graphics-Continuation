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

	virtual void UpdateForce(RigidBody* body, float deltaTime);


private:

};

class Gravity : public RigidForceGen {

	Vector _gravity;

public:
	
	//Create Generator
	Gravity(const Vector &gravity);

	//Apply gravitational force
	virtual void UpdateForce(RigidBody *body, float deltaTime);

};

class ForceRegistry
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

ForceRegistry::ForceRegistry()
{
}

ForceRegistry::~ForceRegistry()
{
}
