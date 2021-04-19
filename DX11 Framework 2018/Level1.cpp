#include "Level1.h"

Level1::Level1(ApplicationGraphics* appGFX) : Scene(appGFX)
{
}

std::vector<SceneObjectsToCreate*> Level1::CreateLevel()
{
	_levelObjects[0]->object = _cube;
	_levelObjects[0]->primitive = _cubePrimitive;
	_levelObjects[0]->position = Vector(0.0f, 0.0f, 0.0f);
	_levelObjects[0]->rotation = Vector(0.0f, 0.0f, 0.0f);
	_levelObjects[0]->modelPath = "Models/cube.obj";
	_levelObjects[0]->texturePath = L"Textures/Crate_COLOUR.dds";
	_levelObjects[0]->specularPath = L"Textures/Crate_SPEC.dds";
	_levelObjects[0]->usesGravity = false;
	_levelObjects[0]->isRigid = true;

	return _levelObjects;
}



