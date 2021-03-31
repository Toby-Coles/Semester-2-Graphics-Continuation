#include "Application.h"




Application::Application()
{
	//initialise variables 
	_pVertexBufferCube = nullptr;
	_pIndexBufferCube = nullptr;
	_pVertexBufferPyramid = nullptr;
	_pIndexBufferPyramid = nullptr;

	appGFX = nullptr;
	_skyMap = nullptr;
	_cube = nullptr;
	
	_camera1 = nullptr;

}

Application::~Application()
{
	Cleanup();
}

HRESULT Application::Initialise(HINSTANCE hInstance, int nCmdShow)
{
	//Initialise graphics object, used for referencing its functionability to be used in the application
	appGFX = new ApplicationGraphics();
	appGFX->Initialize(hInstance, nCmdShow);

	_ui = UserInterface(appGFX);
	
	//Create and set variables for the camera object and set the current active camera in the program
	_camera1 = new Camera();
	
	//Set up force registry and define forces
	_particleForces = new ParticleForceRegister();
	_gravity = Vector(0.0, -9.81f, 0.0f);
	k1 = 0.487f;
	k2 = 0.8f;

	_particleGravityForce = new ParticleGravity(_gravity);
	_dragForce = new ParticleDrag(k1, k2);
	//_gravityForce = new ParticleGravity(_gravity);
	

	_camera1->SetCameraPosition(XMFLOAT3(0.0f, 0.0f, 15.5f));
	_camera1->LookAt(_camera1->GetCameraPosition(), XMFLOAT3(0.0f, 0.0f, 1.0f), _camera1->GetCameraUp());
	_camera1->SetLens(90.0f, 1920 /1080, 0.01f, 1000.0f);

	
	//Create the object for the crate cube in the scene 
	/*_cube = new SceneObject(appGFX, true);
	_cube->_appearance->LoadModelMesh("Models/cube.obj", appGFX->GetDevice());
	_cube->_transform->SetPosition(0.0f, 0.0f, 5.0f);
	_cube->_transform->SetScale(1.0f, 1.0f, 1.0f);
	_cube->_transform->SetRotation(0.1f, 0.1f, 0.1f);
	_cube->_appearance->GenerateTexture(L"Textures/Crate_COLOR.dds", appGFX->GetDevice());
	_cube->_appearance->GenerateTexture(L"Textures/Crate_SPEC.dds", appGFX->GetDevice());
	_worldSceneObjects.push_back(_cube);*/

	//RIGID TEST//
	_cube = new SceneObject(appGFX, true);
	_cube->_appearance->LoadModelMesh("Models/cube.obj", appGFX->GetDevice());
	_cube->_body->SetPosition(Vector(0.0f, 0.0f, 0.0f));
	_cube->_transform->SetPosition(Vector(0.0f, 0.0f, 0.0f));
	_cube->_appearance->GenerateTexture(L"Textures/Crate_COLOR.dds", appGFX->GetDevice());
	_cube->_appearance->GenerateTexture(L"Textures/Crate_SPEC.dds", appGFX->GetDevice());
	_worldSceneObjects.push_back(_cube);

	/*_particleForces->Add(_cube->GetParticle(), _particleGravityForce);
	_particleForces->Add(_cube->GetParticle(), _dragForce);*/

	/*_launchCube = new SceneObject(appGFX);
	_launchCube->_appearance->LoadModelMesh("Models/cube.obj", appGFX->GetDevice());
	_launchCube->_transform->SetPosition(0.0f, -10.0f, 5.0f);
	_launchCube->_transform->SetScale(1.0f, 1.0f, 1.0f);
	_launchCube->_transform->SetRotation(0.1f, 0.1f, 0.1f);
	_launchCube->_appearance->GenerateTexture(L"Textures/Crate_COLOR.dds", appGFX->GetDevice());
	_launchCube->_appearance->GenerateTexture(L"Textures/Crate_SPEC.dds", appGFX->GetDevice());
	_worldSceneObjects.push_back(_cube);*/

	//Initialise the view matrix for the camera
	_camera1->UpdateViewMatrix();
	appGFX->SetEyePosW(_camera1->GetCameraPosition());

	
	//Create the object and initialise the variables for the skybox(skysphere)
	_skyMap = new SceneObject(appGFX, false);
	_skyMap->_appearance->LoadModelMesh("Models/sphere2.obj", appGFX->GetDevice());
	_skyMap->_transform->SetPosition(0.0f, 0.0f, 5.5f);
	_skyMap->_transform->SetScale(100.0f, 100.0f, 100.0f);
	_skyMap->_transform->SetRotation(0.0f, 0.0f, 0.0f);
	_skyMap->_appearance->GenerateTexture(L"Textures/stars_map.dds", appGFX->GetDevice());

	//Creates the ground plane
	_plane = new GroundPlane(appGFX);
	_plane->GeneratePlane(30.0f, 30.0f, 8, 8);
	_plane->_transform->SetPosition (0.0f, -10.0f, 0.0f);
	_plane->_appearance->GenerateTexture(L"Textures/planeSurface.dds", appGFX->GetDevice());
	_showGridPlane = false;
	
	//Initialise the timer in the program
	_timer = new TimeKeep();
	_timer->Reset();
	_timer->Start();

	_moveSpeed = 3.0f;
	
	//Set rotation values
	_rotation = 0.0f;
	_rotationSpeed = 0.5f;
	_earthRotationSpeed = 0.1f;
	appGFX->SetCamera(_camera1);
	return S_OK;
}


HRESULT Application::InitVertexBuffer()
{
	//GenerateGridPlain(3.0f, 3.0f, 6, 4);

	//Hard coded vertex buffer for a cube - NOT IMPLEMENTED INTO SCENE HERE TO DEMONSTRATE UNDERSTANDING AND ABILITY
	HRESULT hrCube;
	SimpleVertex verticesCube[] = {
	
		{ XMFLOAT3(-1.0,  1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(0.0, 0.0) },	//Top Back Left
		{ XMFLOAT3(1.0,  1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(1.0, 0.0) },  	//Top Back Right
		{ XMFLOAT3(-1.0, -1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Back Left
		{ XMFLOAT3(-1.0, -1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Back Left
		{ XMFLOAT3(1.0,  1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(1.0, 0.0) },  	//Top Back Right
		{ XMFLOAT3(1.0, -1.0, -1.0),		XMFLOAT3(0.0,  0.0, -1.0),		XMFLOAT2(1.0, 1.0) },  	//Bottom Back Left
		{ XMFLOAT3(1.0,  1.0, -1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(0.0, 0.0) }, 	//Top Back Right
		{ XMFLOAT3(1.0,  1.0,  1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Right
		{ XMFLOAT3(1.0, -1.0, -1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Back Right
		{ XMFLOAT3(1.0, -1.0, -1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Top Back Right
		{ XMFLOAT3(1.0,  1.0,  1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Right
		{ XMFLOAT3(1.0, -1.0,  1.0),		XMFLOAT3(1.0,  0.0,  0.0),		XMFLOAT2(1.0, 1.0) },  	//Bottom Front Right
		{ XMFLOAT3(1.0,  1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(0.0, 0.0) },  	//Top Front Right
		{ XMFLOAT3(-1.0,  1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Left
		{ XMFLOAT3(1.0, -1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Front Right
		{ XMFLOAT3(1.0, -1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Front Right
		{ XMFLOAT3(-1.0,  1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Left
		{ XMFLOAT3(-1.0, -1.0,  1.0),		XMFLOAT3(0.0,  0.0,  1.0),		XMFLOAT2(1.0, 1.0) },  	//Bottom Front Left
		{ XMFLOAT3(-1.0,  1.0,  1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(0.0, 0.0) }, 	//Top Front Left
		{ XMFLOAT3(-1.0,  1.0, -1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(1.0, 0.0) }, 	//Top Back Left
		{ XMFLOAT3(-1.0, -1.0,  1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(0.0, 1.0) }, 	//Bottom Front Left
		{ XMFLOAT3(-1.0, -1.0,  1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(0.0, 1.0) }, 	//Bottom Front Left
		{ XMFLOAT3(-1.0,  1.0, -1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(1.0, 0.0) }, 	//Top Back Left
		{ XMFLOAT3(-1.0, -1.0, -1.0),		XMFLOAT3(-1.0,  0.0,  0.0),		XMFLOAT2(1.0, 1.0) }, 	//Bottom Back Left
		{ XMFLOAT3(-1.0,  1.0,  1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(0.0, 0.0) }, 	//Top Front Left
		{ XMFLOAT3(1.0,  1.0,  1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Right
		{ XMFLOAT3(-1.0,  1.0, -1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Top Back Left
		{ XMFLOAT3(-1.0,  1.0, -1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Top Back Left
		{ XMFLOAT3(1.0,  1.0,  1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Top Front Right
		{ XMFLOAT3(1.0,  1.0, -1.0),		XMFLOAT3(0.0,  1.0,  0.0),		XMFLOAT2(1.0, 1.0) },  	//Top Back Left
		{ XMFLOAT3(-1.0, -1.0, -1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(0.0, 0.0) },  	//Bottom Back Left
		{ XMFLOAT3(1.0, -1.0, -1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Bottom Back Right
		{ XMFLOAT3(-1.0, -1.0,  1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Front Left
		{ XMFLOAT3(-1.0, -1.0,  1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(0.0, 1.0) },  	//Bottom Front Left
		{ XMFLOAT3(1.0, -1.0, -1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(1.0, 0.0) },  	//Bottom Back Right
		{ XMFLOAT3(1.0, -1.0,  1.0),		XMFLOAT3(0.0, -1.0,  0.0),		XMFLOAT2(1.0, 1.0) },	//Bottom Front Right


	};

	//Create the buffer for the cube
	D3D11_BUFFER_DESC bdCube;
	ZeroMemory(&bdCube, sizeof(bdCube));
	bdCube.Usage = D3D11_USAGE_DEFAULT;
	bdCube.ByteWidth = sizeof(verticesCube);
	bdCube.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdCube.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitDataC;
	ZeroMemory(&InitDataC, sizeof(InitDataC));
	InitDataC.pSysMem = verticesCube;

	hrCube = appGFX->GetDevice()->CreateBuffer(&bdCube, &InitDataC, &_pVertexBufferCube);

	if (FAILED(hrCube))
		return hrCube;

	//Create vertex buffer for pyramid
	HRESULT hrPyramid;
	SimpleVertex verticesPyramid[]{
		//Base
		{XMFLOAT3(-1.0f, 0.0f, -1.0f), XMFLOAT3(-1.0f, 0.0f, -1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f), XMFLOAT3(1.0f, -1.0f, 1.0f)},
		{XMFLOAT3(1.0f,-1.0f,1.0f)	, XMFLOAT3(1.0f,-1.0f,1.0f)},
		{XMFLOAT3(1.0f,-1.0f,-1.0f)	, XMFLOAT3(1.0f,-1.0f,-1.0f)},

		//Point
		{XMFLOAT3(0.0f,1.0f,0.0f) ,XMFLOAT3(0.0f,1.0f,0.0f) },

	};

	D3D11_BUFFER_DESC bdPyramid;
	ZeroMemory(&bdPyramid, sizeof(bdPyramid));
	bdPyramid.Usage = D3D11_USAGE_DEFAULT;
	bdPyramid.ByteWidth = sizeof(verticesPyramid);
	bdPyramid.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bdPyramid.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitDataP;
	ZeroMemory(&InitDataP, sizeof(InitDataP));
	InitDataP.pSysMem = verticesPyramid;

	hrPyramid = appGFX->GetDevice()->CreateBuffer(&bdPyramid, &InitDataP, &_pVertexBufferPyramid);

	if (FAILED(hrPyramid))
		return hrPyramid;




	return S_OK;
}


HRESULT Application::InitIndexBuffer()
{
	//Hard coded index buffers - NOT IMPLEMENTED INTO SCENE HERE TO DEMONSTRATE UNDERSTANDING AND ABILITY
	HRESULT hrCube;

	// Create index buffer for Cube
	WORD indicesCube[] =
	{
		0,1,2 ,3,4,5,
		6,7,8 ,9,10,11,
		12,13,14 ,15,16,17,
		18,19,20 ,21,22,23,
		24,25,26 ,27,28,29,
		30,31,32 ,33,34,35
	};

	D3D11_BUFFER_DESC bdCube;
	ZeroMemory(&bdCube, sizeof(bdCube));

	bdCube.Usage = D3D11_USAGE_DEFAULT;
	bdCube.ByteWidth = sizeof(indicesCube);
	bdCube.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdCube.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory(&InitData, sizeof(InitData));
	InitData.pSysMem = indicesCube;
	hrCube = appGFX->GetDevice()->CreateBuffer(&bdCube, &InitData, &_pIndexBufferCube);

	HRESULT hrPyramid;

	//Create index buffer for Pyramid
	WORD indicesPyramid[]{
		0, 2, 1,
		1, 2, 3,
		0, 1, 4,
		1, 3, 4,
		3, 2, 4,
		2, 0, 4,
	};

	D3D11_BUFFER_DESC bdPyramid;
	ZeroMemory(&bdPyramid, sizeof(bdPyramid));

	bdPyramid.Usage = D3D11_USAGE_DEFAULT;
	bdPyramid.ByteWidth = sizeof(indicesPyramid);
	bdPyramid.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bdPyramid.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitDataP;
	ZeroMemory(&InitDataP, sizeof(InitDataP));
	InitDataP.pSysMem = indicesPyramid;
	hrPyramid = appGFX->GetDevice()->CreateBuffer(&bdPyramid, &InitDataP, &_pIndexBufferPyramid);

	if (FAILED(hrPyramid))
		return hrPyramid;

	return S_OK;
}

void Application::Cleanup()
{
	//Cleanup memory
	if (_pVertexBufferCube) _pVertexBufferCube->Release();
	if (_pIndexBufferCube) _pIndexBufferCube->Release();
	if (_pVertexBufferCube) _pVertexBufferPyramid->Release();
	if (_pIndexBufferCube) _pIndexBufferPyramid->Release();
	if (p_TextureRV) p_TextureRV->Release();
	if (p_SpecularTexture) p_SpecularTexture->Release();
	
}

void Application::Update()
{
	appGFX->UpdateLighting();

	// Update our time
	_timer->Tick();
	float deltaTime = _timer->DeltaTime();

	//Updates the rotation values so they are constant
	_rotation += (_rotationSpeed * deltaTime);
	/*_earthRotation += (_earthRotationSpeed * deltaTime);*/


	//Sets the EyePosw for rendering to that of the active camera
	appGFX->SetEyePosW(appGFX->GetCurrentCamera()->GetCameraPosition());

	//Update forces acting upon particles 
	_particleForces->UpdateForces(deltaTime);



	/*_earth->_transform->SetRotation (XMFLOAT3(0.0f, _earthRotation, 0.0f ));*/

	//Update Scene Objects
	for each (SceneObject* object in _worldSceneObjects)
	{
		object->Update(deltaTime);
		//object->_particle->IntergrateMovement(deltaTime);
	}


	/*if (_cube->_transform->GetPosition()->_y <= -10.0f)
	{
		_cube->_particle->AddForce(Vector(0.0f, 25.81f, 0.0f));
	}*/
	//Constantly sets the skymaps position reletive to the active camera to give the illusion of it never moving
	_skyMap->_transform->SetPosition(appGFX->GetCurrentCamera()->GetCameraPosition().x, appGFX->GetCurrentCamera()->GetCameraPosition().y, appGFX->GetCurrentCamera()->GetCameraPosition().z);
	_skyMap->Update(deltaTime);

	_plane->Update(deltaTime);

	_isWireFrame = appGFX->UpdateWireFrame();

	//Updates all camera control inputs
	UpdateCameraControlls(deltaTime);
	UpdateObjectControlls(deltaTime);

	//Updates ship control inputs
	/*UpdateShipControlls(deltaTime);*/
	
}
void Application::UpdateObjectControlls(float deltaTime) {
	
	if (GetAsyncKeyState('T')) {
		
		//_cube->_particleModel->MoveConstVelocity(deltaTime);
		_cube->_body->AddForce(Vector(10.0f, 0.0f, 0.0f));
	}
	if (GetAsyncKeyState('Y')) {
		
		_cube->_body->AddForce(Vector(-10.0f, 0.0f, 0.0f));
	}
	
}





void Application::UpdateCameraControlls(float deltaTime)
{
	//Camera controlls for W, A, S and D

	//W - S
	if (GetAsyncKeyState('W' ) & 0x8000) _camera1->MoveFowardBack(10.0f * deltaTime);
	else if (GetAsyncKeyState('S') & 0x8000) _camera1->MoveFowardBack(-10.0f * deltaTime);
	
	//A - D
	if (GetAsyncKeyState('A') & 0x8000) _camera1->Strafe(-10.0f * deltaTime);
	else if (GetAsyncKeyState('D') & 0x8000) _camera1->Strafe(10.0f * deltaTime);
	
	//Q-E
	if (GetAsyncKeyState('Q') & 0x8000) _camera1->RotateY(-5.0f * deltaTime);
	else if (GetAsyncKeyState('E') & 0x8000) _camera1->RotateY(5.0f * deltaTime);
	
	//R-F
	if (GetAsyncKeyState('R') & 0x8000) _camera1->Pitch(-5.0f * deltaTime);
	else if (GetAsyncKeyState('F') & 0x8000) _camera1->Pitch(5.0f * deltaTime);



	// ================= Camera Selection ================= //

	if (GetAsyncKeyState('1')) appGFX->SetCamera(_camera1);
	appGFX->SetEyePosW(_camera1->GetCameraPosition());

	//if (GetAsyncKeyState('2')) appGFX->SetCamera(_camera2);
	//appGFX->SetEyePosW(_camera2->GetCameraPosition());

	//if (GetAsyncKeyState('3')) appGFX->SetCamera(_camera3);
	//appGFX->SetEyePosW(_camera3->GetCameraPosition());


	_camera1->UpdateViewMatrix();
	/*_camera2->UpdateViewMatrix();
	_camera3->UpdateViewMatrix();*/
}



void Application::ShowSceneUI()
{
	//// The definition of the scene UI
	//XMFLOAT3 earthScale = XMFLOAT3(_earth->_transform->GetScale());
	//XMFLOAT3 earthPosition = XMFLOAT3(_earth->_transform->GetPosition());
	//XMFLOAT3 shipOrbiterScale = XMFLOAT3(_ship->_transform->GetScale());
	//XMFLOAT3 shipPosition = XMFLOAT3(_ship->_transform->GetPosition());

	ImGui::Begin("Scene Object Control Panel");
	float cubeVelocityX = _cube->_body->GetVelocity()._x;
	float cubeVelocityY = _cube->_body->GetVelocity()._y;
	float cubeVelocityZ = _cube->_body->GetVelocity()._z;

	ImGui::Text("WoodenCube");
	ImGui::SliderFloat("Velocity", &cubeVelocityX , -100.0f, 100.0f);
	ImGui::SliderFloat("Velocity", &cubeVelocityY, -100.0f, 100.0f);
	ImGui::SliderFloat("Velocity", &cubeVelocityZ, -100.0f, 100.0f);

	//ImGui::SliderFloat("Earth Scale X", &earthScale.x, 0.0f, 50.0f);
	//ImGui::SliderFloat("Earth Scale Y", &earthScale.y, 0.0f, 50.0f);
	//ImGui::SliderFloat("Earth Scale Z", &earthScale.z, 0.0f, 50.0f);
	//ImGui::SliderFloat("Earth Rotation", &_earthRotationSpeed, 0.0f, 2.0f);
	//ImGui::SliderFloat("Position X", &earthPosition.x, -50.0f, 50.0f);
	//ImGui::SliderFloat("Position Y", &earthPosition.y, -50.0f, 50.0f);
	//ImGui::SliderFloat("Position Z", &earthPosition.z, -50.0f, 50.0f);
	//ImGui::Text("SpaceShip(Orbiter)");
	//ImGui::SliderFloat("Ship Scale X", &shipOrbiterScale.x, 0.0f, 50.0f);
	//ImGui::SliderFloat("Ship Scale Y", &shipOrbiterScale.y, 0.0f, 50.0f);
	//ImGui::SliderFloat("Ship Scale Z", &shipOrbiterScale.z, 0.0f, 50.0f);
	//ImGui::SliderFloat("Ship Rotation", &_rotationSpeed, 0.0f, 2.0f);
	//ImGui::SliderFloat("Ship Position X", &shipPosition.x, -50.0f, 50.0f);
	//ImGui::SliderFloat("Ship Position Y", &shipPosition.y, -50.0f, 50.0f);
	//ImGui::SliderFloat("ship Position Z", &shipPosition.z, -50.0f, 50.0f);

	ImGui::Text("Grid Plane");
	if (ImGui::Button("Show Grid Plane"))
	{
		if (_showGridPlane == false)
		{
			_showGridPlane = true;
		}
		else
		{
			_showGridPlane = false;
		}
	}

	XMFLOAT3 planeScale = XMFLOAT3(_plane->_transform->GetScale()->_x, _plane->_transform->GetScale()->_y, _plane->_transform->GetScale()->_z);


	ImGui::SliderFloat("Grid Plane Scale X", &planeScale.x, 0.0f, 50.0f);
	
	ImGui::SliderFloat("Grid Plane Scale Z", &planeScale.z, 0.0f, 50.0f);
	ImGui::End();

	_plane->_transform->SetScale(planeScale.x, planeScale.y, planeScale.z);
	ImGui::Begin("Controls");
	ImGui::Text("===/ Camera \===");
	ImGui::Text("W: Fly Fowards");
	ImGui::Text("A: Fly Left");
	ImGui::Text("S: Fly Backwards");
	ImGui::Text("D: Fly Right");
	ImGui::Text("Q: Rotate Left");
	ImGui::Text("E: Rotate Right");
	ImGui::Text("R: Pitch Up");
	ImGui::Text("F: Pitch Down");
	ImGui::Text("F2: Wireframe");
	ImGui::Text("Z: Flashlight (SpotLight)");
	
	ImGui::End();

	//Sets the data that may have been altered by the UI
	/*_earth->_transform->SetScale(earthScale);
	_earth->_transform->SetPosition(earthPosition);
	_ship->_transform->SetScale(shipOrbiterScale);
	_ship->_transform->SetPosition(shipPosition);*/
}



void Application::Draw()
{
	//Change the rasterizer state to render textures on the outside of the objects
	_ui.FrameBegin();
	
	appGFX->SetSkyboxRasterizerState(false);
	
	ShowSceneUI();
	appGFX->ClearBackBuffer();
	//Draw objects
	appGFX->SetPixelShader(appGFX->GetScenePixelShader());

	appGFX->SetWireFrame(_isWireFrame);

	for each (SceneObject* object in _worldSceneObjects)
	{
		object->Draw();
	}

	if (_showGridPlane)
	{
		_plane->Draw(_plane->GetPlaneVb(), _plane->GetPlaneIb(), _plane->GetPlaneIndexCount());

	}

	appGFX->RunLightingControls();

	//Set the rasterizer state to draw the textures on the inside of the model for the skybox

	appGFX->SetSkyboxRasterizerState(true);
	

	appGFX->SetPixelShader(appGFX->GetSkyboxPixelShader());

	_skyMap->Draw();

	//skyMap->Draw();
	_ui.FrameEnd();

	appGFX->Present();

	//HARD CODED RENDERING ///////////////////////////////////////////////////////////////////////
	/*XMFLOAT4X4 world = appGFX->GetWorld();
	Render all planets
	if (cubeView == true)
	{

		for (int i = 0; i < 5; i++)
		{

			world = XMLoadFloat4x4(&sceneObjects[i]);
			cb.mWorld = XMMatrixTranspose(world);
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

			_pImmediateContext->DrawIndexed(36, 0, 0);
		}
	}
	else if (pyramidView == true)
	{
		for (int i = 0; i < 5; i++)
		{

			appGFX->GetWorld()  = XMLoadFloat4x4(&sceneObjects[i]);
			appGFX->GetCB().mWorld = XMMatrixTranspose(world);
			_pImmediateContext->UpdateSubresource(_pConstantBuffer, 0, nullptr, &cb, 0, 0);

			_pImmediateContext->DrawIndexed(18, 0, 0);
		}
	}*/


	
}