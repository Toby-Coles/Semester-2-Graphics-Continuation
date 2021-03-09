#pragma once

#include "ParticleModel.h"
#include <vector>

	class ParticleForceGenerator
	{
	public:
		ParticleForceGenerator();
		~ParticleForceGenerator();

		//Overloaded interface to update the force applied to a given particle
		virtual void UpdateForce(ParticleModel* particle, float deltaTime) = 0;
	private:


	};

	// **============ Gravity ============** // 
	class ParticleGravity : public ParticleForceGenerator {

		//Holds gravitational accelleration 
		Vector _gravity;
		
	public:
		//Greates generator with given accelleration
		ParticleGravity(const Vector &gravity);

		virtual void UpdateForce(ParticleModel *particle, float deltaTime);
	};

	// **============ Drag ============** // 
	class ParticleDrag : public ParticleForceGenerator
	{
		// Velocity Drag Coefficient
		float _k1;

		//Velocity squared drag coefficient 
		float _k2;

	public:

		/** Creates the generator with the given coefficients. */
		ParticleDrag(float k1, float k2);

		/** Applies the drag force to the given particle. */
		virtual void UpdateForce(ParticleModel *particle, float deltaTime);
	};

	class ParticleForceRegister {

	protected:

		//Struct for each force, containing the particle itself along with the force generation
		struct ParticleForceObject
		{
			ParticleModel* particle;
			ParticleForceGenerator* forceGen;
		};

		typedef std::vector<ParticleForceObject> Registry;
		Registry _registrations;

	public:

		//Adds a force generator to the given particle
		void Add(ParticleModel* particle, ParticleForceGenerator* forceGen);

		//Removes the pair from the registry
		void Remove(ParticleModel* particle, ParticleForceGenerator* forceGen);

		void Clear();

		void UpdateForces(float deltaTime); 
	};



