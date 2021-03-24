#include "RigidBodyForceGenerator.h"

//RigidForceGen::RigidForceGen()
//{
//}
//
//RigidForceGen::~RigidForceGen()
//{
//}



// **============ Gravity ============** // 
RigidGravity::RigidGravity(const Vector& gravity) : _gravity(gravity)
{

}

void RigidGravity::UpdateForce(RigidBody* body, float deltaTime) {

	if (body->GetMass() <= 0) return;

	body->AddForce(_gravity * body->GetMass());

}

void RigidForceRegister::Add(RigidBody* body, RigidForceGen* fg)
{
	RigidForceRegister::ForceRegistration registration;
	registration.body = body;
	registration.fg = fg;
	registrations.push_back(registration);
}

void RigidForceRegister::Remove(RigidBody* body, RigidForceGen* fg)
{
}

void RigidForceRegister::Clear()
{
	registrations.clear();
}

void RigidForceRegister::UpdateForces(float deltaTime)
{
	Registry::iterator i = registrations.begin();
	for (; i != registrations.end(); i++)
	{
		i->fg->UpdateForce(i->body, deltaTime);
	}
}


