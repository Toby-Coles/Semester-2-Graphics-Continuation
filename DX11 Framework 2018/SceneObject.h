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

#include <vector>

using namespace DirectX;

class SceneObject
{
public:
	SceneObject(ApplicationGraphics* graphics);
	SceneObject();
	~SceneObject();

	/*void GenerateTexture(wchar_t* texturePath, ID3D11Device* device);
	void LoadModelMesh(char* filepath, ID3D11Device* device);*/
	
	
	virtual void Draw();
	virtual void Draw(ID3D11Buffer* vertexBuffer, ID3D11Buffer* indexBuffer, UINT indexCount);
	void Update(); 
	void Initialise();

	//HRESULT LoadTexture(wchar_t* path, ID3D11ShaderResourceView** texture, ID3D11Device* device);
	ApplicationGraphics* appGFX;

	Transform* _transform;
	Appearance* _appearance;
private:
	

protected:
	


	//MeshData mMeshData;

	

	//std::vector<ID3D11ShaderResourceView*> mTextures;


	

};

