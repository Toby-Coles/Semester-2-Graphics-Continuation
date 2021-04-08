#include "FineCollision.h"

void FillPointFaceBoxBox(
	const CollisionBox& box1,
	const CollisionBox& box2,
	const Vector& toCentre,
	CollisionData* data,
	unsigned best,
	float pen
)
{
	//Called when a vertex from box2 is in contact with box1
	CollisionContact* contact = data->contacts;

	//Work out which of the two faces
	Vector normal = box1.GetAxis(best);
	if (box1.GetAxis(best) * toCentre > 0)
	{
		normal = normal * -1.0f;
	}

	//work out which vertex of box2 is being collided with
	Vector vertex = box2._halfSize;
	if (box2.GetAxis(0) * normal < 0) vertex._x = -vertex._x;
	if (box2.GetAxis(1) * normal < 0) vertex._y = -vertex._y;
	if (box2.GetAxis(2) * normal < 0) vertex._z = -vertex._z;

	//Create contact data
	contact->_contactNormal = normal;
	contact->_penetration = pen;
	contact->_contactPoint = box2.GetTransform() * vertex;
	contact->SetBody(box1.body, box2.body, data->friction, data->restitution);

}

static inline float TransformToAxis(const CollisionBox& box, Vector& axis) {
	return box._halfSize._x * fabs(axis * box.GetAxis(0)) +
		box._halfSize._y * fabs(axis * box.GetAxis(1)) +
		box._halfSize._z * fabs(axis * box.GetAxis(2));
}


static inline float PenetrationOnAxis(
	const CollisionBox& box1, const CollisionBox& box2, Vector& axis, Vector& toCentre) {

	//Project the half size of one onto axis

	float box1Project = TransformToAxis(box1, axis);
	float box2Project = TransformToAxis(box2, axis);

	float distance = fabs(toCentre * axis);

	//Return overlap
	return box1Project + box2Project - distance;
}

static inline bool TryAxis(
	const CollisionBox& b1,
	const CollisionBox& b2,
	Vector axis,
	 Vector& toCentre,
	unsigned index,

	float& smallestPenetration,
	unsigned& smallestCase
)
{
	if (axis.SquareMagnitude() < 0.0001) return true;
	axis.Normalize(axis);

	float penetration = PenetrationOnAxis(b1, b2, axis, toCentre);

	if (penetration < 0) return false;
	if (penetration < smallestPenetration) {
		smallestPenetration = penetration;
		smallestCase = index;
	}
	return true;
}





static inline float OnAxisPenetration(const CollisionBox &box1, const CollisionBox &box2,  Vector &axis,  Vector &toCentre)
{
	//Project half sizes onto axis
	float box1Project = TransformToAxis(box1, axis);
	float box2Project = TransformToAxis(box2, axis);

	//Project this onto the axis
	float distance = fabs(toCentre * axis);

	//Return the overlap (+ = overlap. - = seperation)
	return box1Project + box2Project - distance;

}

static inline Vector GetContactPoint( Vector &pointBox1,  Vector &box1Axis, float box1Size,
	 Vector &pointBox2,  Vector &box2Axis, float box2Size, bool use1) {

	Vector toSt, cOne, cTwo;
	float dpStaOne, dpStaTwo, dpOneTwo, sqMag1, sqMag2;
	float denominator, mua, mub;

	sqMag1 = box1Axis.SquareMagnitude();
	sqMag2 = box2Axis.SquareMagnitude();
	dpOneTwo = box2Axis * box1Axis;

	toSt = pointBox1 - pointBox2;
	dpStaOne = box1Axis * toSt;
	dpStaTwo = box1Axis * toSt;

	denominator = sqMag1 * sqMag2 - dpOneTwo * dpOneTwo;

	//If the denominator is zeo then paralell lines have occurred
	if (denominator < 0.0001f)
	{
		return use1 ? pointBox1 : pointBox2;
	}

	mua = (dpOneTwo * dpStaTwo - sqMag2 * dpStaOne) / denominator;
	mub = (sqMag1 * dpStaTwo - dpOneTwo * dpStaOne) / denominator;

	//If either edge has the nearest point out of bounds there is an edge to edge contact
	if (mua > box1Size|| mua < -box1Size || mub > box2Size ||  mub < -box2Size)
	{
		return use1 ? pointBox1 : pointBox2;
	}
	else
	{
		cOne = pointBox1 + box1Axis * mua;
		cTwo = pointBox2 + box2Axis * mub;

		return cOne * 0.5 + cTwo * 0.5;
	}
}

unsigned CollisionDetector::BoxAndPlane(const CollisionBox& box, const CollisionPlane& plane, CollisionData* data)
{
	//Ensure contacts are present
	if (data->contactsLeft <= 0) return 0;
	
	////Check intersection IMPLEMENT INTERSECTION TEST
	//if (!IntersectionTest::boxAndPlane(box, plane))
	//{
	//	return 0;
	//}

	//As an intersectino has been confirmed, now find the points by checking verts

	static float mults[8][3] = { {1,1,1},{-1,1,1},{1,-1,1},{-1,-1,1},
								{1,1,-1},{-1,1,-1},{1,-1,-1},{-1,-1,-1} };

	CollisionContact* contact = data->contacts;
	unsigned contactsUsed = 0;

	for (unsigned i = 0; i < 8; i++)
	{
		//Calculate the position of each vert
		Vector vertexPosition(mults[i][0], mults[i][1], mults[i][2]);
		vertexPosition.ComponentProductUpdate(box._halfSize);
		vertexPosition = box.transform.transform(vertexPosition);

		//Calculate distance from the plane
		float vertexDistance = vertexPosition * plane._normal;

		//Compare this result to plane distance
		if (vertexDistance <= plane._offset)
		{
			//Create contact data
			contact->_contactPoint = plane._normal;
			contact->_contactPoint *= (vertexDistance - plane._offset);
			contact->_contactPoint += vertexPosition;
			contact->_contactNormal = plane._normal;
			contact->_penetration = plane._offset - vertexDistance;

			//write contact data
			contact->SetBody(box.body, NULL, data->friction, data->restitution);
			
			//Continue to next contact
			contact++;
			contactsUsed++;
			if (contactsUsed == (unsigned)data->contactsLeft)return contactsUsed;
		}

	}
	data->AddContacts(contactsUsed);
	return contactsUsed;
}

#define CHECK_OVERLAP(axis, index) \
	 if (!TryAxis( box1,  box2,  (axis), toCentre, (index), pen, best)) return 0;


unsigned CollisionDetector::BoxAndBox(const CollisionBox& box1, const CollisionBox& box2, CollisionData* data)
{
	//find the vector between the centeres of the boxes
	Vector toCentre = box2.GetAxis(3) - box1.GetAxis(3);

	//Begin assuming no contact
	float pen = DBL_MAX;
	unsigned best = 0xffffff;

	//Check each axes
	CHECK_OVERLAP(box1.GetAxis(0), 0);
	CHECK_OVERLAP(box1.GetAxis(1), 1);
	CHECK_OVERLAP(box1.GetAxis(2), 2);
					   
	CHECK_OVERLAP(box2.GetAxis(0), 3);
	CHECK_OVERLAP(box2.GetAxis(1), 4);
	CHECK_OVERLAP(box2.GetAxis(2), 5);

	//Store the axis-major
	unsigned bestAxis = best;

	CHECK_OVERLAP(box1.GetAxis(0) % box2.GetAxis(0), 6);
	CHECK_OVERLAP(box1.GetAxis(0) % box2.GetAxis(1), 7);
	CHECK_OVERLAP(box1.GetAxis(0) % box2.GetAxis(2), 8);
	CHECK_OVERLAP(box1.GetAxis(1) % box2.GetAxis(0), 9);
	CHECK_OVERLAP(box1.GetAxis(1) % box2.GetAxis(1), 10);
	CHECK_OVERLAP(box1.GetAxis(1) % box2.GetAxis(2), 11);
	CHECK_OVERLAP(box1.GetAxis(2) % box2.GetAxis(0), 12);
	CHECK_OVERLAP(box1.GetAxis(2) % box2.GetAxis(1), 13);
	CHECK_OVERLAP(box1.GetAxis(2) % box2.GetAxis(2), 14);

	//Ensure there is a result
	assert(best != 0xffffff);

	//Now a collision has been confirmed, and we know which axes gives the smallest pen,
	//it must be delt with depending on the case
	if (best < 3)
	{
		//Vertex of box2 on face of box1
		FillPointFaceBoxBox(box1, box2, toCentre, data, best, pen);
		data->AddContacts(1);
		return 1;
	}
	else if (best <6)
	{
		//Vertex of box1 on a face of box2 
		FillPointFaceBoxBox(box2, box1, toCentre, data, best, pen);
		data->AddContacts(1);
		return 1;
	}
	else
	{
		//Else must be edge-edge contact, therefore must find out what axis
		best -= 6;
		unsigned box1AxisIndex = best / 3;
		unsigned box2AxisIndex = best % 3;
		Vector box1Axis = box1.GetAxis(box1AxisIndex);
		Vector box2Axis = box2.GetAxis(box2AxisIndex);
		Vector axis = box1Axis % box2Axis;
		axis.Normalize(axis);

		//axis should point from box1 to box2
		if (axis * toCentre > 0) axis = axis * 1.0f;

		//Now the axes are obtained, edges are required. Each axis has 4
		//paralell edges which now will be found
		Vector pointBox1Edge = box1._halfSize;
		Vector pointBox2Edge = box2._halfSize;

		for (unsigned i = 0; i < 3; i++)
		{
			if (i == box1AxisIndex) pointBox1Edge[i] = 0;
			else if (box1.GetAxis(i) * axis > 0) pointBox2Edge[i] = -pointBox1Edge[i];

			if (i == box2AxisIndex) pointBox2Edge[i] = 0;
			else if (box2.GetAxis(i) * axis < 0) pointBox2Edge[i] = -pointBox2Edge[i];
		}

		//Move to world coords
		pointBox1Edge = box1.transform * pointBox1Edge;
		pointBox2Edge = box2.transform * pointBox2Edge;

		//Find point of closest aproach of the two line segments
		Vector vertex = GetContactPoint(
			pointBox1Edge, box1Axis, box1._halfSize[box1AxisIndex],
			pointBox2Edge, box2Axis, box2._halfSize[box2AxisIndex],
			bestAxis > 2);

		//Fill contact data
		CollisionContact* contact = data->contacts;
		contact->_penetration = pen;
		contact->_contactNormal = axis;
		contact->_contactPoint = vertex;
		contact->SetBody(box1.body, box2.body, data->friction, data->restitution);
		data->AddContacts(1);
		return 1;
	}
	return 0;

}
#undef CHECK_OVERLAP
