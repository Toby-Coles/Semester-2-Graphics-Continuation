#include "CourseCollision.h"



BoundingSphere::BoundingSphere(const Vector& centre, float radius)
{
}

BoundingSphere::BoundingSphere(BoundingSphere& one,  BoundingSphere& two)
{
	Vector centreOffset = two.centre - one.centre;
	float distance = centreOffset.SquareMagnitude();
	float radiusDiff = two.radius - one.radius;

	//Check if the larger sphere encloses the smaller one 
	if (radiusDiff * radiusDiff >= distance)
	{
		if (one.radius > two.radius)
		{
			centre = one.centre;
			radius = one.radius;
		}
		else
		{
			centre = two.centre;
			radius = two.radius;
		}
	}

	//Otherwise, we must work with partially overlapping spheres
	else
	{
		distance = sqrt(distance);
		radius = (distance + one.radius + two.radius) * ((float)0.5);

		//The new centre will be based on ones centre, moved towards two's by an ammount proportional to the radii
		centre = one.centre;
		if (distance > 0)
		{
			centre += centreOffset * ((radius - one.radius) / distance);
		}
	}
}

bool BoundingSphere::Overlaps( BoundingSphere* other) 
{
	float distanceSq = (centre - other->centre).SquareMagnitude();
	return distanceSq < (radius + other->radius)* (radius + other->radius);
}


