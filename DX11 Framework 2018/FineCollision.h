#pragma once
#ifndef COLLISION_FINE
#define COLLISION_FINE

#include "CollisionContacts.h"
#include "RigidBody.h"

struct CollisionData
{
	//Holds the contact array to be written
	CollisionContact *contacts;

	CollisionContact *contactArray;

	//Holds the max contacts the array can hold 
	int contactsLeft;

	//Contacts so far
	unsigned contactCount;

	//Holds friction value 
	float friction;

	//Holds resitution value
	float restitution;

	float tolerance;

	void AddContacts(unsigned count)
	{
		//Reduce number of contacts remaining and add used contacts
		contactsLeft -= count;
		contactCount += count;

		//Move the array forwards
		contacts += count;
	}

	void Reset(unsigned count) {
		contactsLeft = count;
		contactCount = 0;
		contacts = contactArray;
	}

	bool CheckRemainingContacts() {
		return contactsLeft > 0;
	}

};

class ColliderPrimitive
{
public:

	friend class CollisionDetector;
	
	RigidBody *body;
	Matrix3x4 offset;

	//Possibly  move later on
	//void GenerateContacts(const ColliderPrimitive &P1, const ColliderPrimitive &P2, CollisionData *data);

	Vector GetAxis(unsigned index) const
	{
		return transform.GetAxisVector(index);
	}

	const Matrix3x4& GetTransform()const {
		return transform;
	}

	void CalculateTransform();

	Matrix3x4 transform;

private:

protected:
	

};



// ================= Collision Types ================= // 
class CollisionPlane {
public:
	Vector _normal;
	float _offset;
};

class CollisionBox : public ColliderPrimitive
{
	public:
		Vector _halfSize;
};

// ================= Collider Scenareos ================= //
// Wrapper class containing collision detection routines

class CollisionDetector {
public:
	
	//Box and plane collision
	static unsigned BoxAndPlane(
		const CollisionBox& box,
		const CollisionPlane& plane,
		CollisionData* data);


	//Box and Box Collision
	static unsigned BoxAndBox(
		const CollisionBox &box1,
		const CollisionBox &box2,
		CollisionData *data);
};

//class IntersectionTests
//{
//	static bool BoxAndBox(const CollisionBox& box1, const CollisionBox& box2);
//};

#endif