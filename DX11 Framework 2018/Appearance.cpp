#include "Appearance.h"

Appearance::Appearance()
{
}

Appearance::~Appearance()
{
}



void Appearance::GenerateTexture(wchar_t* texturePath, ID3D11Device* device)
{
	ID3D11ShaderResourceView* texture;
	LoadTexture(texturePath, &texture, device);
	mTextures.push_back(texture);
}

void Appearance::LoadModelMesh(char* filepath, ID3D11Device* device)
{
	//mMeshadata = incomingdata
	mMeshData = OBJLoader::Load(filepath, device, false);
}

HRESULT Appearance::LoadTexture(wchar_t* path, ID3D11ShaderResourceView** texture, ID3D11Device* device)
{
	return CreateDDSTextureFromFile(device, path, nullptr, texture);

}
