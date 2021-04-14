#pragma once
#ifndef COLLISION_CONTACTS
#define COLLISION_CONTACTS
#include "Vector.h"
#include "RigidBody.h"

class ContactResolver;

class CollisionContact
{
public:
	//Provides CollisionContact with accessibility to ContactResolver
	friend class ContactResolver;

	//Holds the bodies in the contact
	RigidBody* body[2];

	//The position of the contact in world coords
	Vector _contactPoint;

	//the direction of the cotnact in world coords
	Vector _contactNormal;

	//Depth of penetration at the contact point
	float _penetration;
	
	//holds lateral frictino coefficient
	float _friction;

	//holds the nonrmal resitution coefficient
	float _restitution;
	

	//Holds the world space position of the contact point relative to the centre of each body. 
	Vector _relativeContactPos[2];


	void SetBody(RigidBody* b1, RigidBody* b2, float friction, float restitution);

	//Calculates the orthonormal basis for the contact point
	void CalculateContactBasis();

	void GenerateContacts(unsigned int contactCount);

	Vector CalculateFrictionlessImpulse(Matrix3x3* inverseInertiaTensor);
	Vector CalculateFrictionImpulse(Matrix3x3* inverseInertiaTensor);

	Vector CalculateLocalVelocity(unsigned bodyIndex, float deltaTime);
	void CalculateDesiredDeltaVelocity(float deltaTime);

	void ApplyVelocityChange(Vector velocity[2], Vector rotation[2]);
	void ApplyNonlinearProjection(Vector linearChange[2], Vector angularChange[2], float penetration);

private:
protected:
	//Calculates internal data from state data. Called before any resolution algorithms. 
	void CalculateInternals(float deltaTime);

	//A transform matrix which converts coords from the contacts frame of reference into world coordinates
	Matrix3x3 contactToWorldMatrix;

	//Holds the closing velocity at the point of contact. This is set when CalculateInternals runs
	Vector _contactVelocity;

	//Holds the required change in velocity for the contact to be resolved
	float _desiredDeltaVelocity; 

	//Updates the awake state of rigidbodies taking place in a given contact
	void MatchAwakeState();

	//Reverses the contact, swapping two rigid bodies and reversing their contact normals
	void SwapBodies();

	

	//Stores the number of position iterations used in the previous call to resolve contacts
	
};

// =================== Contact resolution routine =================== //
class ContactResolver
{
public:

	ContactResolver(unsigned iterations, float velocityEpsilon = (float) 0.01f, float positionEpsilon = (float)0.01f);


	void ResolveContacts(CollisionContact *contactArray, unsigned contactCount, float deltaTime);

	void PrepareContacts(CollisionContact* contactArray, unsigned contactCount, float deltaTime);

	void AdjustPositions(CollisionContact* contactArray, unsigned contactCount, float deltaTime);

	void AdjustVelocities(CollisionContact* contactArray, unsigned contactCount, float deltaTime);

protected:
	//To avoid penetration instability, default as 0.01; (larger number will visually interpenetrate)
	float _positionEpsilon;


	//Holds the number of iterations to use when resolving position
	unsigned _positionIterations;

	unsigned _positionIterationsUsed;

	unsigned _velocityIterations;
	unsigned _velocityIterationsUsed;
	float _velocityEpsilon;


private:

};

class ContactGenerator {
public: 
	virtual unsigned AddContact(CollisionContact* contact, unsigned limit);
};


#endif // !COLLISION_CONTACTS
