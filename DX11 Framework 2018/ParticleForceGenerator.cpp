#include "ParticleForceGenerator.h"

ParticleForceGenerator::ParticleForceGenerator()
{
}

ParticleForceGenerator::~ParticleForceGenerator()
{
}


// **============ Gravity ============** // 
ParticleGravity::ParticleGravity(const Vector& gravity) : _gravity(gravity)
{

}
void ParticleGravity::UpdateForce(ParticleModel* particle, float deltaTime) {
	//ensure particle does not have infinite mass
	if (particle->GetMass() <= 0) return;

	particle->AddForce(_gravity * particle->GetMass());

}

// **============ Drag ============** // 
ParticleDrag::ParticleDrag(float k1, float k2) : _k1(k1), _k2(k2)
{

}

void ParticleDrag::UpdateForce(ParticleModel* particle, float deltaTime) {
	Vector force;
	particle->GetVelocity(&force);

	//Calculate Drag Coefficient 
	float dragCoefficient = force.Magnitude(force);

	dragCoefficient = _k1 * dragCoefficient + _k2 * dragCoefficient * dragCoefficient;

	//Calculate and apply the final drag force
	force.Normalize(force);
	force *= -dragCoefficient;
	particle->AddForce(force);

}

// **============ ParticleRegister ============** // 

void ParticleForceRegister::Add(ParticleModel* particle, ParticleForceGenerator* forceGen)
{
	ParticleForceObject registerObject;
	registerObject.particle = particle;
	registerObject.forceGen = forceGen;
	_registrations.push_back(registerObject);
}

void ParticleForceRegister::Remove(ParticleModel* particle, ParticleForceGenerator* forceGen)
{
}

void ParticleForceRegister::Clear()
{
	_registrations.clear();
}

void ParticleForceRegister::UpdateForces(float deltaTime)
{
}
