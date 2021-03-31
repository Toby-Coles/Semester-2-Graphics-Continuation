#pragma once
#ifndef COLLISION_FINE
#define COLLISION_FINE

#include "CollisionContacts.h"
#include "RigidBody.h"

struct CollisionData
{
	//Holds the contact array to be written
	CollisionContact *contacts;

	//Holds the max contacts the array can hold 
	int contactsLeft;
};

class ColliderPrimitive
{
public:
	RigidBody *body;
	Matrix3x4 offset;

private:

};


#endif