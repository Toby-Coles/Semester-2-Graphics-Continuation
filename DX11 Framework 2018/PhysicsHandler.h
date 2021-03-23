#pragma once
// Handles registration management for rigid bodies and particles
#include <vector>
#include "RigidBody.h"

class PhysicsHandler
{
public:
	PhysicsHandler();
	~PhysicsHandler();

	typedef std::vector<RigidBody*> RigidBodies;

	//Process scene physics
	void HandlePhysics();

	//Reset accumulators and prepare body for use
	void FirstFrame();



private:

protected:

	//Holds simulated bodies
	RigidBodies _bodies;

};

