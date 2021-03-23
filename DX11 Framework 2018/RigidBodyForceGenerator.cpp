#include "RigidBodyForceGenerator.h"

//RigidForceGen::RigidForceGen()
//{
//}
//
//RigidForceGen::~RigidForceGen()
//{
//}



// **============ Gravity ============** // 
Gravity::Gravity(const Vector& gravity) : _gravity(gravity)
{

}

void Gravity::UpdateForce(RigidBody* body, float deltaTime) {

	if (body->GetMass() <= 0) return;

	body->AddForce(_gravity * body->GetMass());

}

void ForceRegistry::Add(RigidBody* body, RigidForceGen* fg)
{
	ForceRegistry::ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void ForceRegistry::UpdateForces(float deltaTime)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->UpdateForce(i->body, deltaTime);
	}
}
