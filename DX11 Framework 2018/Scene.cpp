#include "Scene.h"

Scene::Scene(ApplicationGraphics* appGFX)
{
	_graphics = appGFX;
	Initialize();
}

void Scene::Initialize()
{
	
}

void Scene::LoadLevel(std::vector<SceneObject*> objects, std::vector<CollisionBox*> primitives)
{
	for each (SceneObject * object in objects) {
		
		_sceneObjects.push_back(object);
	}

	for each (CollisionBox * box in primitives) {
		_boxPrimitives.push_back(box);
	}
}

// ========== Scene Object Management ========== // 
void Scene::AddRigidBody(SceneObject* object, char* modelPath, wchar_t texturePath, wchar_t specularPath, Vector position, Vector rotation, bool usesGravity)
{
	object = new SceneObject();
	object->_appearance->LoadModelMesh(modelPath, _graphics->GetDevice());
	object->_appearance->GenerateTexture(&texturePath, _graphics->GetDevice());
	object->_appearance->GenerateTexture(&specularPath, _graphics->GetDevice());

	object->_transform->SetPosition(position);
	object->_transform->SetRotation(rotation);

	object->CreatePhysics();


}

void Scene::AddParticle(char name, char modelPath, wchar_t texturePath, Vector position, Vector rotation, bool usesGravity)
{
}

void Scene::SetPrimitiveToBodyBox(RigidBody* body, CollisionBox* box)
{
	box = new CollisionBox();
	//Set default half size
	box->_halfSize = Vector(1.0f, 1.0f, 1.0f);
	box->body = body;
	_boxPrimitives.push_back(*box);
}

void Scene::Update(float deltaTime)
{
}

void Scene::UnloadScene()
{
}

// ========== Collisions ========== // 
void Scene::ConfigureCollisionSystems()
{
}

void Scene::UpdateCollisionData()
{
}
