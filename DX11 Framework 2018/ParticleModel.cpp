#include "ParticleModel.h"
ParticleModel::ParticleModel(Transform* transform)
{
	_transform = transform;
	_position = *transform->GetPosition();
	_netForce._x = 0.0f; _netForce._y = 0.0f; _netForce._z = 0.0f;
	SetMass(2.0);
	_dampening = 0.2f;
	_velocity._x = 0.0;	_velocity._y = 0.0;	_velocity._z = 0.0;
	_accelleration._x = 0.5;  _accelleration._y = 0.5;  _accelleration._z = 0.5;
	_gravity = 9.81f;
	_vector = new Vector();
	_useGravity = true;
	

}

ParticleModel::~ParticleModel()
{
}


void ParticleModel::MoveConstVelocity( float deltaTime)
{
	Vector previousPosition = *_transform->GetPosition();

	_transform->SetPosition( previousPosition + _velocity * deltaTime);

}

void ParticleModel::MoveConstAccelleration(float deltaTime) {
	Vector previousPosition = *_transform->GetPosition();
	Vector previousVelocity = _velocity;

	_transform->SetPosition(previousPosition + previousVelocity * deltaTime + _accelleration.operator*(0.5f) * deltaTime * deltaTime);
	_velocity = previousVelocity + _accelleration.
		operator*(deltaTime);

}

void ParticleModel::SetVelocity(float x, float y, float z)
{
	_velocity._x = x;
	_velocity._y = y;
	_velocity._z = z;
}

void ParticleModel::GetVelocity(Vector* velocity)
{
	*velocity = ParticleModel::_velocity;
}

void ParticleModel::SetMass(float mass)
{
	assert(mass != 0);
	ParticleModel::_inverseMass = ((float)1.0) / _mass;
}



void ParticleModel::UpdateNetForce()
{
	// ======== Calculate Net Force from Forces ======== //
	_netForce._x = _thrustForce._x + _friction._x + _breakForce._x;
	_netForce._y = _thrustForce._y + _friction._y + _breakForce._y - _gravity;
	_netForce._z = _thrustForce._z + _friction._z + _breakForce._z ;

	
}

void ParticleModel::UpdateAccelleration()
{
	_accelleration._x = _netForce._x / _mass;
	_accelleration._y = _netForce._y / _mass;
	_accelleration._z = _netForce._z / _mass;
}

void ParticleModel::AddThrust(Vector thrust)
{
	_thrustForce += thrust;
}

void ParticleModel::AddBReaking(Vector breaking)
{
	_breakForce += breaking;
}

void ParticleModel::ApplyGravity()
{
	_accelleration._y += _gravity;
}

void ParticleModel::AddForce(const Vector& force)
{
	_forceAccumulator += force;
}

void ParticleModel::IntergrateMovement(float deltaTime)
{
	if (_inverseMass <= 0.0f) return;

	assert(deltaTime > 0.0);

	//Linear position
	_position.AddScaledVector(_velocity, deltaTime);

	//Accelleration
	Vector resultingAccelleration = _accelleration;
	resultingAccelleration.AddScaledVector(_forceAccumulator, _inverseMass);

	//Update linear velocity from the accelleration
	_velocity.AddScaledVector(resultingAccelleration, deltaTime);

	//Impose Drag
	_velocity *= pow(_dampening, deltaTime);

	//Clear the forces so they do not effect the next intergration
	_forceAccumulator.Clear();

}

void ParticleModel::Move()
{
	Vector previousPosition = *_transform->GetPosition();
	Vector newPosition;
	Vector previousVelocity = _velocity;

	//Update world position having added displacement to previous position
	//MAKE MORE EFFICIENT
	newPosition._x = previousPosition._x + previousVelocity._x * _deltaTime + 0.5f * _accelleration._x * _deltaTime * _deltaTime;
	newPosition._y = previousPosition._y + previousVelocity._y * _deltaTime + 0.5f * _accelleration._y * _deltaTime * _deltaTime;
	newPosition._z = previousPosition._z + previousVelocity._z * _deltaTime + 0.5f * _accelleration._z * _deltaTime * _deltaTime;
	_transform->SetPosition(newPosition);
	
	_velocity._x = previousVelocity._x * _dampening + _accelleration._x * _deltaTime;
	_velocity._y = previousVelocity._y * _dampening + _accelleration._y * _deltaTime;
	_velocity._z = previousVelocity._z * _dampening + _accelleration._z * _deltaTime;
	

}

void ParticleModel::Update(float deltaTime)
{
	_deltaTime = deltaTime;
	UpdateNetForce();
	UpdateAccelleration();
	Move();
	if (_useGravity)
	{
		ApplyGravity();
	}
}
