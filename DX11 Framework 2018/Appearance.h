#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "OBJLoader.h"
#include "DDSTextureLoader.h"
#include <vector>

using namespace DirectX;

class Appearance
{
public:
	Appearance();
	~Appearance();

	void GenerateTexture(wchar_t* texturePath, ID3D11Device* device);
	void LoadModelMesh(char* filepath, ID3D11Device* device);
	HRESULT LoadTexture(wchar_t* path, ID3D11ShaderResourceView** texture, ID3D11Device* device);

	MeshData GetMeshData() const { return mMeshData; }
	std::vector<ID3D11ShaderResourceView*> GetTextureVector() const { return mTextures; }

private:

	MeshData mMeshData;
	std::vector<ID3D11ShaderResourceView*> mTextures;
};



