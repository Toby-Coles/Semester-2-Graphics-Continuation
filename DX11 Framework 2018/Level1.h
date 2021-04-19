#pragma once
#include "Scene.h"
#include "ApplicationGraphics.h"


// The level scene for  == TEST 1 == 
class Level1 : public Scene
{
public:
	Level1(ApplicationGraphics* appGFX) :Scene(appGFX) {};
	std::vector<SceneObjectsToCreate*> CreateLevel() override;

private:

	// =========== Object Storage =========== //
	std::vector<SceneObjectsToCreate*> _levelObjects;
	

	// =========== Objects =========== // 
	SceneObject* _cube; 
	CollisionBox* _cubePrimitive;
};
