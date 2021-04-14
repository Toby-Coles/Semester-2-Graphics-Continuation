#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <directxcolors.h>
#include "Vector.h"

using namespace DirectX;

class Transform
{
public:
	Transform();
	~Transform();


	void SetPosition(Vector position);
	void SetPosition(float x, float y, float z);
	void SetRotation(Vector rotation);
	void SetRotation(float x, float y, float z);
	void SetScale(Vector scale);
	void SetScale(float x, float y, float z);
	void SetTransform(XMFLOAT4X4 transform);
	void SetTransform(XMMATRIX transform);

	XMMATRIX UpdateTransforms();
	XMFLOAT4X4 GetTransform();
	Vector* GetPosition();

	Vector* GetRotation();
	Vector* GetScale();
	


private:

	XMFLOAT4X4 mTransform;
	Vector mPosition;
	Vector mRotation;
	Vector mScale;
};



