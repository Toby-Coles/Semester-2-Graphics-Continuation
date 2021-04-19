#include "CollisionContacts.h"

void CollisionContact::SetBody(RigidBody* b1, RigidBody* b2, float friction, float restitution)
{
	CollisionContact::body[0] = b1;
	CollisionContact::body[1] = b2;
	CollisionContact::_friction = friction;
	CollisionContact::_restitution = restitution;
}

void CollisionContact::CalculateContactBasis()
{
	Vector contactTangent[2];

	//Check if Z axis is closer to the x or y axis
	if (fabs(_contactNormal._x) > fabs(_contactNormal._y))
	{
		//Scaling factor to ensure normalization
		const float scalingFactor = (float)1.0f / sqrtf(_contactNormal._z * _contactNormal._z + _contactNormal._x * _contactNormal._x);

		
		contactTangent[0]._x = _contactNormal._z * scalingFactor;
		contactTangent[0]._y = 0;
		contactTangent[0]._z = -_contactNormal._x * scalingFactor;

		contactTangent[1]._z = _contactNormal._y * contactTangent[0]._x;
		contactTangent[1]._y = _contactNormal._z * contactTangent[0]._x -
			_contactNormal._x * contactTangent[0]._z;
		contactTangent[1]._z = -_contactNormal._y * contactTangent[0]._x;
	}
	else
	{
			//Scaling factor to ensure normalization
		const float scalingFactor = (float)1.0 / sqrtf(_contactNormal._z * _contactNormal._z + _contactNormal._y * _contactNormal._y);

		//X acis at right angles to the world's X axis
		contactTangent[0]._x = 0;
		contactTangent[0]._y = -_contactNormal._z * scalingFactor;
		contactTangent[0]._z = _contactNormal._y * scalingFactor;

		//Y axis at right angles to new x and z axis
		contactTangent[1]._x = _contactNormal._y * contactTangent[0]._z -
			_contactNormal._z * contactTangent[0]._y;
		contactTangent[1]._y = -_contactNormal._x * contactTangent[0]._z;
		contactTangent[1]._z = _contactNormal._x * contactTangent[0]._y;
	}

	//Creata a matrix fron these new three vectors
	contactToWorldMatrix.SetComponents(_contactNormal, contactTangent[0], contactTangent[1]);

					
}

void CollisionContact::GenerateContacts(unsigned int contactCount)
{
	unsigned limit = contactCount;
	/*CollisionContact* nextContact = contacts; */
}

Vector CollisionContact::CalculateFrictionlessImpulse(Matrix3x3* inverseInertiaTensor)
{
	Vector impulseContact;
	Vector deltaVelocityWorld = _relativeContactPos[0] % _contactNormal;

	deltaVelocityWorld = inverseInertiaTensor[0].transform(deltaVelocityWorld);
	deltaVelocityWorld = deltaVelocityWorld % _relativeContactPos[0];

	//Process change in velocity in contact coords
	float deltaVelocity = deltaVelocityWorld * _contactNormal;

	//Add linear component of change in velocity
	deltaVelocity += body[0]->GetInverseMass();

	if (body[1]) {
		//Go back through transformation sequence
		Vector deltaVelWorld = _relativeContactPos[1] % _contactNormal;
		deltaVelWorld = inverseInertiaTensor[1].transform(deltaVelWorld);
		deltaVelWorld = deltaVelWorld % _relativeContactPos[1];

		// Add change in velocity from rotation
		deltaVelocity += deltaVelWorld * _contactNormal;

		// Add  change in velocity from linear motion
		deltaVelocity += body[1]->GetInverseMass();
	}

	//Calculate impulse size
	impulseContact._x = _desiredDeltaVelocity / deltaVelocity;
	impulseContact._y = 0;
	impulseContact._z = 0;

	if (impulseContact._x >= 1000.0f || impulseContact._y >= 1000.0f || impulseContact._x >= 1000.0f)
	{
		return Vector(0.0f, 0.0f, 0.0f);
	}

	return impulseContact;
}

Vector CollisionContact::CalculateFrictionImpulse(Matrix3x3* inverseInertiaTensor)
{
	Vector contactImpulse;
	float inverseMass = body[0]->GetInverseMass();
	
	//Skew symmetric matrix built for converting between linear and angular quantities
	Matrix3x3 impulseToTorque;
	impulseToTorque.SetSkewSymmetric(_relativeContactPos[0]);

	//Build the matrix to convert contactImpulse to change in velocity(world coords)
	Matrix3x3 deltaVelocityWorld = impulseToTorque;
	deltaVelocityWorld *= inverseInertiaTensor[0];
	deltaVelocityWorld *= impulseToTorque;
	deltaVelocityWorld *= -1;

	//Check if we need to do the same with the seecond body
	if (body[1])
	{
		impulseToTorque.SetSkewSymmetric(_relativeContactPos[1]);

		//Calculate velocity change matrix
		Matrix3x3 deltaVelocityWorldBody2 = impulseToTorque;
		deltaVelocityWorldBody2 *= inverseInertiaTensor[1];
		deltaVelocityWorldBody2 *= impulseToTorque;
		deltaVelocityWorldBody2 *= -1;

		//Add to total delta velocity
		deltaVelocityWorld += deltaVelocityWorldBody2;

		//Add Inverse Mass 
		inverseMass += body[1]->GetInverseMass();
	}

	//Convert into contact coordinates
	Matrix3x3 deltaVelocity = contactToWorldMatrix.Transpose();
	deltaVelocity *= deltaVelocityWorld;
	deltaVelocity *= contactToWorldMatrix;

	//Add in the change in linear velocity
	deltaVelocity.data[0] += inverseMass;
	deltaVelocity.data[4] += inverseMass;
	deltaVelocity.data[8] += inverseMass;

	//Invert to get the implusse needed
	Matrix3x3 impulseMatrix = deltaVelocity.Inverse();

	//Find target velocities to cancel
	Vector cancelVelocities(_desiredDeltaVelocity, -_contactVelocity._y, -_contactVelocity._z);

	//Find impulse to cancel target velocities
	contactImpulse = impulseMatrix.transform(cancelVelocities);

	//Check for exeeding friction ammounts
	float planarImpulse = sqrtf(contactImpulse._y * contactImpulse._y + contactImpulse._z * contactImpulse._z);

	if (planarImpulse > contactImpulse._x * _friction)
	{
		//Use dynamic friction
		contactImpulse._y /= planarImpulse;
		contactImpulse._z /= planarImpulse;

		contactImpulse._x = deltaVelocity.data[0] + deltaVelocity.data[1] * _friction * contactImpulse._y +
			deltaVelocity.data[2] * _friction * contactImpulse._z;
		contactImpulse._x = _desiredDeltaVelocity / contactImpulse._x;
		contactImpulse._y *= _friction * contactImpulse._x;
		contactImpulse._z *= _friction * contactImpulse._x;

	}

	return contactImpulse;
}

Vector CollisionContact::CalculateLocalVelocity(unsigned bodyIndex, float deltaTime)
{
	RigidBody* activeBody = body[bodyIndex];

	//Calculate Velocity of contact point
	Vector contactVelocity = activeBody->GetRotation() % _relativeContactPos[bodyIndex];
	contactVelocity += activeBody->GetVelocity();

	//Convert velocity into contact coords
	contactVelocity = contactToWorldMatrix.TransformTranspose(contactVelocity);

	//Calculate velocity 
	Vector accelVelocity = activeBody->GetLastFrameAccelleration() * deltaTime;

	//Calculate velocity in contact coords
	accelVelocity = contactToWorldMatrix.TransformTranspose(accelVelocity);

	// As we only need planar acceleration, ignore contact noral accelleration compoents
	accelVelocity._x = 0;

	//Add the velocities 
	contactVelocity += accelVelocity;

	return contactVelocity;

}

void CollisionContact::CalculateDesiredDeltaVelocity(float deltaTime)
{
	const static float velocityLimit = (float)0.25f;
	float velocityFromAcceleration = 0;

	if (body[0]->GetAwake())
	{
		velocityFromAcceleration +=
			body[0]->GetLastFrameAccelleration() * deltaTime * _contactNormal;
	}

	if (body[1] && body[1]->GetAwake())
	{
		velocityFromAcceleration -=
			body[1]->GetLastFrameAccelleration() * deltaTime * _contactNormal;
	}

	//If the velocity is slow, limit restitution
	float thisRestitution = _restitution;
	if (fabs(_contactVelocity._x) < velocityLimit)
	{
		thisRestitution = (float)0.0f;
	}

	_desiredDeltaVelocity = -_contactVelocity._x - thisRestitution * (_contactVelocity._x - velocityFromAcceleration);
}


void CollisionContact::ApplyVelocityChange(Vector velocity[2], Vector rotation[2])
{
	//Get the inverse mass and inertia tensor in world coords

	Matrix3x3 inverseInertiaTensor[2];
	body[0]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[0]);
	if (body[1])
	{
		body[1]->GetInverseInertiaTensorWorld(&inverseInertiaTensor[1]);
	}

	//calculate impulse for each contact axis
	Vector contactImpulse;
	if (_friction == (float)0.0f)
	{
		//Use short calculation format for frictionless case
		contactImpulse = CalculateFrictionlessImpulse(inverseInertiaTensor);
	}
	else
	{
		contactImpulse = CalculateFrictionImpulse(inverseInertiaTensor);
	}

	//convert impulse into world coords
	Vector impulse = contactToWorldMatrix.transform(contactImpulse);

	//Split impulse into linear and rotational components
	Vector impulsiveTorque = _relativeContactPos[0] % impulse;
	rotation[0] = inverseInertiaTensor[0].transform(impulsiveTorque);
	velocity[0].Clear();
	velocity[0].AddScaledVector(impulse, body[0]->GetInverseMass());

	//Apply changes
	body[0]->AddVelocity(velocity[0]);
	body[0]->AddRotation(rotation[0]);

	if (body[1])
	{
		Vector impulsiveTorque = _relativeContactPos[1] % impulse;
		rotation[1] = inverseInertiaTensor[1].transform(impulsiveTorque);
		velocity[1].Clear();
		velocity[1].AddScaledVector(impulse, -body[1]->GetInverseMass());

		body[1]->AddVelocity(velocity[1]);
		body[1]->AddRotation(rotation[1]);
	}
}

void CollisionContact::ApplyNonlinearProjection(Vector linearChange[2], Vector angularChange[2], float penetration)
{
	const float angularLimit = (float)0.2f;
	float angular[2];
	float linear[2];
	
	float totalInertia = 0;
	float linearInertia[2];
	float angularInertia[2];

	//Work out inertia of objects in contact normal direction
	for (unsigned i = 0; i < 2; i++) if (body[i])
	{
		Matrix3x3 inverseTensor;
		body[i]->GetInverseInertiaTensorWorld(&inverseTensor);

		Vector angularInertiaworld = _relativeContactPos[i] % _contactNormal;

		angularInertiaworld = inverseTensor.transform(angularInertiaworld);

		angularInertiaworld = angularInertiaworld % _relativeContactPos[i];

		angularInertia[i] = angularInertiaworld * _contactNormal;

		//Linear component is inverse mass
		linearInertia[i] = body[i]->GetInverseMass();


		//Keep track of the total inertia
		totalInertia += linearInertia[i] + angularInertia[i];
	}

	//loop through again to calculate and apply changes
	for (unsigned i = 0; i < 2; i++) /*if (body[i])*/
	{
		float sign = (i == 0) ? 1 : -1;
	
		angular[i] = sign * penetration * (angularInertia[i] / totalInertia);
		linear[i] = sign * penetration * (linearInertia[i] / totalInertia);
		

		//To avoid too great of angular projections, limit angular move
		Vector projection = _relativeContactPos[i];
		projection.AddScaledVector(_contactNormal, -_relativeContactPos[i].ScalarProduct(_contactNormal));

		//Use small angle approximation for the sine of the angle
		float maxMagnitude = angularLimit * projection.Magnitude();

		if (angular[i] < -maxMagnitude)
		{
			float totalMovement = angular[i] + linear[i];
			angular[i] = -maxMagnitude;
			linear[i] = totalMovement - angular[i];
		}
		else if (angular[i] > maxMagnitude)
		{
			float totalMovement = angular[i] + linear[i];
			angular[i] = maxMagnitude;
			linear[i] = totalMovement - angular[i];
		}

		//Calculate desired rotation 
		if (angular[i] == 0)
		{
			angularChange[i].Clear();
		}
		else
		{
			//Work out desired rotation direction
			Vector angularTargetDirection = _relativeContactPos[i].VectorProduct(_contactNormal);

			Matrix3x3 inverseInertiaTensor;
			body[i]->GetInverseInertiaTensorWorld(&inverseInertiaTensor);

			//Calculate direction needed to rotate in
			angularChange[i] = inverseInertiaTensor.transform(angularTargetDirection) * (angular[i] / angularInertia[i]);

		}

		//Velocity change is just the linear movement along the contact normal
		linearChange[i] = _contactNormal * linear[i];

		//Begin to apply the calculated values
		Vector pos;
		body[i]->GetPosition(&pos);
		pos.AddScaledVector(_contactNormal, linear[i]);
		
		body[i]->SetPosition(pos + linearChange[i]);

		//Change orientation
		Quaternion q;
		q = body[i]->GetOrientation();
		q.addScaledVector(angularChange[i], ((float)1.0f));
		body[i]->SetOrientation(q);

		
		//Calculate the derived data for any body which is asleep, so the changes are reflected in the objects data. 
		if (!body[i]->GetAwake()) body[i]->CalculateDerivedData();

	}
}

void CollisionContact::CalculateInternals(float deltaTime)
{
	//Check first object is null, and swap if it is
	if (!body[0]) SwapBodies();
	assert(body[0]);

	//Calculate set of axes at the contact point
	CalculateContactBasis(); 

	//Store relative position of th contact to each body
	_relativeContactPos[0] = _contactPoint - body[0]->GetPosition();
	if (body[1])
	{
		_relativeContactPos[1] = _contactPoint - body[1]->GetPosition();
	}

	//Find relative velocity of bodies at contact point
	_contactVelocity = CalculateLocalVelocity(0, deltaTime);
	if (body[1])
	{
		_contactVelocity -= CalculateLocalVelocity(1, deltaTime);
	}

	//Calculate the desired change in velocity 
	CalculateDesiredDeltaVelocity(deltaTime);
}

void CollisionContact::MatchAwakeState()
{
	// Collisions with the world never cause a body to wake up.
	if (!body[1]) return;

	bool body0awake = body[0]->GetAwake();
	bool body1awake = body[1]->GetAwake();

	// Wake up only the sleeping one
	if (body0awake ^ body1awake) {
		if (body0awake) body[1]->SetAwake(true);
		else body[0]->SetAwake(true);
	}

}

void CollisionContact::SwapBodies()
{
	_contactNormal *= -1;
	RigidBody *temp = body[0];
	body[0] = body[1];
	body[1] = temp;
}

ContactResolver::ContactResolver(unsigned iterations, float velocityEpsilon, float positionEpsilon)
{
	_positionIterations = iterations;
	_velocityIterations = iterations;
	_velocityEpsilon = velocityEpsilon;
	_positionEpsilon = positionEpsilon;


}

void ContactResolver::ResolveContacts(CollisionContact* contactArray, unsigned contactCount, float deltaTime)
{
	if (contactCount == 0) return;

	

	//Prepare contacts for processing
	PrepareContacts(contactArray, contactCount, deltaTime);

	//Resolve interpenetration problems
	AdjustPositions(contactArray, contactCount, deltaTime);

	//Resolve veolcity problems with contacts
	AdjustVelocities(contactArray, contactCount, deltaTime);
}

void ContactResolver::PrepareContacts(CollisionContact* contactArray, unsigned contactCount, float deltaTime)
{
	//Generate contact velocity and axis information
	CollisionContact* lastContact = contactArray + contactCount;

	for (CollisionContact* contact = contactArray; contact < lastContact; contact++)
	{
		//Calculate the internal contact data
		contact->CalculateInternals(deltaTime);
	}
}

void ContactResolver::AdjustPositions(CollisionContact* contactArray, unsigned contactCount, float deltaTime)
{
	unsigned i, index;
	Vector linear[2], angular[2];
	float max;
	Vector deltaPosition;
	
	//Iteratively solve interpenetrations in order of severity (depth)
	_positionIterationsUsed = 0;

	while (_positionIterationsUsed < _positionIterations)
	{
		//Find bigest penetration
		max = _positionEpsilon;
		index = contactCount;
		for  (i = 0; i < contactCount; i++)
		{
			if (contactArray[i]._penetration > max)
			{
				max = contactArray[i]._penetration;
				index = i;
			}
		}
		
		if (index == contactCount) break;

		//Match the awake state at the contact
		contactArray[index].MatchAwakeState();

		//Resolve penetration
		contactArray[index].ApplyNonlinearProjection(linear, angular, max);

		//This may have changed the penetration of other bodies, therefore update contacts
		for (i = 0; i < contactCount; i++) {

			//Check each body in the contact
			for (unsigned b = 0; b < 2; b++) if (contactArray[i].body[b]) {

				//Check for a match with each body in the new contact
				for (unsigned d = 0; d < 2; d++) {
					if (contactArray[i].body[b] == contactArray[index].body[d])
					{
						deltaPosition = linear[d] + angular[d].VectorProduct(contactArray[i]._relativeContactPos[b]);

						contactArray[i]._penetration += deltaPosition.ScalarProduct(contactArray[i]._contactNormal) * (b ? 1 : -1);
					}
				}
			}
		}
		_positionIterationsUsed++;
	}

	
}

void ContactResolver::AdjustVelocities(CollisionContact* contactArray, unsigned contactCount, float deltaTime)
{
	Vector velocity[2], rotation[2];
	Vector deltaVelocity;

	_velocityIterationsUsed = 0;

	//iteratively handle impacts in order of severity(Depth)
	while (_velocityIterationsUsed < _velocityIterations) {
		float max = _velocityEpsilon;

		unsigned index = contactCount;
		for (unsigned i = 0; i < contactCount; i++)
		{
			if (contactArray[i]._desiredDeltaVelocity > max)
			{
				max = contactArray[i]._desiredDeltaVelocity;
				index = i;
			}
		}
		if (index == contactCount) break;
		//Match the awake state at contact
		contactArray[index].MatchAwakeState();

		//Do resolution with the contact which came out on top
		contactArray[index].ApplyVelocityChange(velocity, rotation);

		//Recompute closing velocities
		for (unsigned i = 0; i < contactCount; i++)
		{
			//Check each body
			for (unsigned b = 0; b < 2; b++)
			{
				//check for a match with eaech body in newly resolved contact
				for (unsigned d = 0; d < 2; d++)
				{
					if (contactArray[i].body[b] == contactArray[index].body[b]);
					{
						deltaVelocity = velocity[d] + rotation[d].VectorProduct(contactArray[i]._relativeContactPos[b]);

						// Adjust signs
						contactArray[i]._contactVelocity += contactArray[i].contactToWorldMatrix.TransformTranspose(deltaVelocity)
							* (b ? -1 : 1);

						contactArray[i].CalculateDesiredDeltaVelocity(deltaTime);
					}
				}
			}
		}
		_velocityIterationsUsed++;
	}
	
}
