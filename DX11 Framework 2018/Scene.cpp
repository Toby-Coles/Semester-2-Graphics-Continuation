#include "Scene.h"

Scene::Scene(ApplicationGraphics* appGFX)
{
	_graphics = appGFX;
	Initialize();
}

void Scene::Initialize()
{
	//Force generators and physics
	_particleForces = new ParticleForceRegister();
	_rigidForces = new RigidForceRegister();
	_gravity = Vector(0.0f, -9.81f, 0.0f);
	_rigidGravity = new RigidGravity(_gravity);

	
	//Contacts & Collisions
	_contactResolver = new ContactResolver(10);
	_collisionData = new CollisionData();
	_collisionData->contactArray = _contactArray;
}

void Scene::LoadLevel(std::vector<SceneObjectsToCreate*> objects)
{
	UnloadScene();
	for each (SceneObjectsToCreate * object in objects) {
		if (object->isRigid)
		{
			AddRigidBody(object->object, object->modelPath, object->texturePath, object->specularPath, object->position, object->rotation, object->usesGravity);
		}
		else
		{
			AddParticle(object->object, object->modelPath, object->texturePath, object->specularPath, object->position, object->rotation, object->usesGravity);
		}
	/*	if (primitives.empty())
		{
			OutputDebugStringA("Primitives Vector Empty");
		}
		else
		{
			for each (CollisionBox * box in primitives) {
				_boxPrimitives.push_back(box);
			}
		}*/
		_sceneObjects.push_back(object->object);
	}

	
	
}

// ========== Scene Object Management ========== // 
void Scene::AddRigidBody(SceneObject* object, char* modelPath, wchar_t texturePath, wchar_t specularPath, Vector position, Vector rotation, bool usesGravity)
{
	//Create and set up a new rigidbody with passed in values
	SceneObject* newObject = object;

	newObject = new SceneObject();
	newObject->CreatePhysics();

	newObject->_appearance->LoadModelMesh(modelPath, _graphics->GetDevice());
	newObject->_appearance->GenerateTexture(&texturePath, _graphics->GetDevice());
	newObject->_appearance->GenerateTexture(&specularPath, _graphics->GetDevice());

	newObject->_transform->SetPosition(position);
	newObject->_transform->SetRotation(rotation);
	if (usesGravity)
	{
		_rigidForces->Add(newObject->_body, _rigidGravity);
	}
	SetPrimitiveToBodyBox(newObject->_body, newObject->_colliderBox);

}

void Scene::AddParticle(SceneObject* object, char* modelPath, wchar_t texturePath, wchar_t specularPath, Vector position, Vector rotation, bool usesGravity)
{
}

void Scene::SetPrimitiveToBodyBox(RigidBody* body, CollisionBox* box)
{
	box = new CollisionBox();
	//Set default half size
	box->_halfSize = Vector(1.0f, 1.0f, 1.0f);
	box->body = body;
	_boxPrimitives.push_back(box);
}

void Scene::Update(float deltaTime)
{
	//Update Primitives
	for each (CollisionBox * box in _boxPrimitives) {
		box->CalculateTransform();
	}

	//Update Scene Objects
	for each (SceneObject * object in _sceneObjects)
	{
		object->Update(deltaTime);
	}

	//Update Collision Data
	UpdateCollisions();

}

void Scene::UnloadScene()
{
	_sceneObjects.clear();
	_boxPrimitives.clear();

}

// ========== Collisions ========== // 
void Scene::ConfigureCollisionSystems()
{
}

void Scene::UpdateCollisions()
{

}

//Updates to enable the first object in the vector to be controllable to the user for each scene (if they choose)
void Scene::UpdateMoveControlls()
{
	if (GetAsyncKeyState('T')) {

		//_cube->_particleModel->MoveConstVelocity(deltaTime);
		_sceneObjects[0]->_body->AddForce(Vector(100.0f, 0.0f, 0.0f));
	}
	if (GetAsyncKeyState('G')) {

		_sceneObjects[0]->_body->AddForce(Vector(-100.0f, 0.0f, 0.0f));
	}

	if (GetAsyncKeyState('H')) {

		
		_sceneObjects[0]->_body->AddForce(Vector(0.0f, 0.0f, 100.0f));
	}
	if (GetAsyncKeyState('F')) {

		_sceneObjects[0]->_body->AddForce(Vector(0.0f, 0.0f, -100.0f));
	}
}
