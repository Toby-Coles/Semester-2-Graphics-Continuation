#pragma once

#include "ApplicationGraphics.h"
#include "RigidBody.h"
#include "SceneObject.h"
#include "Vector.h"

#include "CollisionContacts.h"
#include "FineCollision.h"

//The scene class acts as a system for easily building scenes in the engine
//Enabling the user to easily add and update objects
//The inherited level files are used to load the levels into the currently active scene

class Scene
{
public:
	Scene(ApplicationGraphics* appGFX);
	void Initialize();
	~Scene();

	// ========== Scene Object Management ========== // 
	void LoadLevel(std::vector<SceneObject*>objects, std::vector<CollisionBox*>primitives);
	void AddRigidBody(SceneObject* object, char* modelPath, wchar_t texturePath, wchar_t specularPath, Vector position, Vector rotation, bool usesGravity);
	void AddParticle(char name, char modelPath, wchar_t texturePath, Vector position, Vector rotation, bool usesGravity);
	void SetPrimitiveToBodyBox(RigidBody* body, CollisionBox* box); 
	
	void Update(float deltaTime);
	void UnloadScene();

	// ========== Collisions ========== // 
	void ConfigureCollisionSystems();
	void UpdateCollisionData();



private:
	ApplicationGraphics* _graphics;

	// ======== Scene Vectors ======== // 
	std::vector<SceneObject*> _sceneObjects; 
	std::vector<CollisionBox*> _boxPrimitives;

	// ======== Particle Forces ======== //
	ParticleForceRegister* _particleForces;
	ParticleGravity* _particleGravityForce;

	// ======== RigidBody forces ======== //
	RigidForceRegister* _rigidForces;
	RigidGravity* _rigidGravity;
	Vector _gravity;

	// ======== Contacts & Collisions ======== //
	const static unsigned _maxContacts = 256;
	ContactResolver* _contactResolver;
	CollisionData* _collisionData;
	CollisionContact _contactArray[_maxContacts];
};

