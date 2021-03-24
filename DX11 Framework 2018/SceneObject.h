#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
//#include "DDSTextureLoader.h"
//#include "OBJLoader.h"
#include "ApplicationGraphics.h"
//#include "Application.h"
#include "Transform.h"
#include "Appearance.h"
#include "ParticleModel.h"
#include "ParticleForceGenerator.h"
#include "RigidBody.h"
#include "RigidBodyForceGenerator.h"

#include <vector>

using namespace DirectX;

class SceneObject
{
public:
	SceneObject(ApplicationGraphics* graphics, bool isRigid);
	SceneObject();
	~SceneObject();

	
	virtual void Draw();
	virtual void Draw(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
	void Update(float deltaTime); 
	void Initialise();

	ParticleModel* GetParticle() const { return _particle; }

	ApplicationGraphics* appGFX;

	RigidBody* _body;
	Transform* _transform;
	Appearance* _appearance;
	ParticleModel* _particle;
	
private:
	float _deltaTime;
	bool isRigidBody;


	ParticleForceRegister* _forces;
	ParticleGravity* _gravity;

protected:
	
};

