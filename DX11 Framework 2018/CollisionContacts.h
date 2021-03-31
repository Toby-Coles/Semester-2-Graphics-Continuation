#pragma once
#ifndef COLLISION_CONTACTS
#define COLLISION_CONTACTS
#include "Vector.h"


class CollisionContact
{
public:
	//The position of the contact in world coords
	Vector _contactPoint;

	//the direction of the cotnact in world coords
	Vector _contactNormal;

	//Depth of penetration at the contact point
	float penetration;


private:

};




#endif // !COLLISION_CONTACTS
