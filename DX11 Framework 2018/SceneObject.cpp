#include "SceneObject.h"

SceneObject::SceneObject (ApplicationGraphics* graphics) {

	
	this->appGFX = graphics;

	//ID3D11Device* device = appGFX->GetDevice();
	//mMeshData = OBJLoader::Load(filepath, this->_pd3dDevice, false);

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


	//Set initial Values
	XMStoreFloat4x4(&_transform->GetTransform() , XMMatrixIdentity());
	
	
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


}

void SceneObject::GenerateTexture(wchar_t* texturePath, ID3D11Device* device)
{
	ID3D11ShaderResourceView* texture;
	LoadTexture(texturePath, &texture, device);
	mTextures.push_back(texture);
}

void SceneObject::LoadModelMesh(char* filepath, ID3D11Device* device)
{
	//mMeshadata = incomingdata
	mMeshData = OBJLoader::Load(filepath, device, false);

}



HRESULT SceneObject::LoadTexture(wchar_t* path, ID3D11ShaderResourceView** texture, ID3D11Device* device)
{
	return CreateDDSTextureFromFile(device, path, nullptr, texture); 

}



void SceneObject::Draw()
{
	
	appGFX->SetIndexBuffer(mMeshData.IndexBuffer);
	appGFX->SetVertexBuffer(mMeshData.VertexBuffer);

	for (int i = 0; i < mTextures.size(); i++)
	{
		appGFX->BindTextures(i, 1, mTextures);
	}
	
	
	
	//appGFX->InitShadersAndInputLayout();
	
	appGFX->UpdateConstantBufferVariables(_transform->GetTransform());
	appGFX->Draw(mMeshData.IndexCount);
}
void SceneObject::Draw(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer,UINT indexCount){
	appGFX->SetIndexBuffer(indexBuffer);
	appGFX->SetVertexBuffer(vertexBuffer);
	appGFX->UpdateConstantBufferVariables(_transform->GetTransform());

	appGFX->Draw(indexCount);
}

void SceneObject::Update()
{
	_transform->UpdateTransforms();
}





