#include "SceneObject.h"

SceneObject::SceneObject (ApplicationGraphics* graphics) {

	
	this->appGFX = graphics;
	


	Initialise();

}

//second constructor for if a model does not need to be loaded 
SceneObject::SceneObject() {

	Initialise();
	
}

SceneObject::~SceneObject()
{
}

void SceneObject::Initialise()
{
	_transform = new Transform();
	_appearance = new Appearance();

	
	


	//Set initial Values
	XMStoreFloat4x4(&_transform->GetTransform() , XMMatrixIdentity());
	
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


}

void SceneObject::CreatePhysics()
{
	if (isRigidBody)
	{
		_body = new RigidBody(_transform);
		isRigidBody = true;

	}
	else
	{
		isRigidBody = false;
		_particle = new ParticleModel(_transform);
	}
}

void SceneObject::Draw()
{
	
	appGFX->SetIndexBuffer(_appearance->GetMeshData().IndexBuffer);
	appGFX->SetVertexBuffer(_appearance->GetMeshData().VertexBuffer);

	for (int i = 0; i < _appearance->GetTextureVector().size(); i++)
	{
		appGFX->BindTextures(i, 1, _appearance->GetTextureVector());
	}
	
	
	
	//appGFX->InitShadersAndInputLayout();
	
	appGFX->UpdateConstantBufferVariables(_transform->GetTransform());
	appGFX->Draw(_appearance->GetMeshData().IndexCount);
}
void SceneObject::Draw(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,UINT indexCount){
	appGFX->SetIndexBuffer(indexBuffer);
	appGFX->SetVertexBuffer(vertexBuffer);
	appGFX->UpdateConstantBufferVariables(_transform->GetTransform());

	appGFX->Draw(indexCount);
}

void SceneObject::Update(float deltaTime)
{
	if (isRigidBody)
	{
		_body->Update(deltaTime);
		
	}
	else
	{
		_particle->Update(deltaTime);

	}

	_transform->UpdateTransforms();
	//_forces->UpdateForces(deltaTime);
}





