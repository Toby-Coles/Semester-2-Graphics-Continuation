#pragma once

#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "resource.h"
#include <vector>
#include "DDSTextureLoader.h"
#include "Structures.h"
#include "SceneObject.h"
#include "ApplicationGraphics.h"
#include "Time.h"
#include "Camera.h"
#include "CubeObject.h"
#include "GroundPlane.h"
#include "SetUpGUI.h"

#include "ParticleForceGenerator.h"
#include "RigidBodyForceGenerator.h"

#include "CollisionContacts.h"
#include "FineCollision.h"


using namespace DirectX;

class Application
{
private:

	//Hard Coded shape values
	ID3D11Buffer* _pVertexBufferCube;
	ID3D11Buffer* _pIndexBufferCube;
	ID3D11Buffer* _pVertexBufferPyramid;
	ID3D11Buffer* _pIndexBufferPyramid;

	SimpleVertex simpleVertex;

	//Texturing
	ID3D11ShaderResourceView* p_TextureRV = nullptr;
	ID3D11ShaderResourceView* p_SpecularTexture = nullptr;

	//Update Controls
	/*void UpdateShipControlls(float deltaTime);*/
	void UpdateCameraControlls(float deltaTime);
	void UpdateObjectControlls(float deltaTime);

	ApplicationGraphics* appGFX;
	SceneObject* _skyMap;

	//Scene Objects (old, for hard coded values)
	//std::vector<XMFLOAT4X4> sceneObjects;

	XMFLOAT4X4 _gridPlane;

	std::vector<SceneObject*> _worldSceneObjects;

	//Scene Objects
	GroundPlane* _plane;
	SceneObject* _cube;
	SceneObject* _cube2;

	SceneObject* _launchCube;

	UserInterface _ui;

	Camera* _camera1;
	Camera* _camera2;
	Camera* _camera3;
private:

	// ======== Particle Forces ======== //
	ParticleForceRegister* _particleForces;
	ParticleGravity* _particleGravityForce;

	// ======== RigidBody forces ======== //
	RigidForceRegister* _rigedForces;

	// ======== Contacts & Collisions ======== //
	const static unsigned _maxContacts = 256;
	ContactResolver* _contactResolver;
	CollisionData* _collisionData;
	CollisionContact _contactArray[_maxContacts];

	
	CollisionBox
		_box1Primitive;
	CollisionBox _box2Primitive;



		ParticleDrag* _dragForce;
		float k1, k2;

		Vector _gravity;

		bool _isWireFrame = false;
		void Cleanup();
		bool _showGridPlane;

		HRESULT InitVertexBuffer();
		HRESULT InitIndexBuffer();
		TimeKeep* _timer;

		//Scene Objects
		std::vector<SceneObject> _applicationObjects;

		float _rotationSpeed;
		float _earthRotationSpeed;

		float _earthRotation = 0.0f;
		float _rotation = 0.0f;
		XMFLOAT3 _offset = XMFLOAT3(0.0f, 3.0f, 5.0f);

		float _moveSpeed;

	public:
		Application();
		~Application();

		HRESULT Initialise(HINSTANCE hInstance, int nCmdShow);

		void ShowSceneUI();

		//Rigid Body Methods
		



		void Update();
		void Draw();
	};



