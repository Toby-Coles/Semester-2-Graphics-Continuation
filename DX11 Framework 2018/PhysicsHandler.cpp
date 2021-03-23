#include "PhysicsHandler.h"

void PhysicsHandler::FirstFrame()
{
	for (RigidBodies::iterator b = _bodies.begin();
		b != _bodies.end();
		b++)
	{
		b->ClearAccumulators();
		b->CalculateDerivedData();
	}
}
