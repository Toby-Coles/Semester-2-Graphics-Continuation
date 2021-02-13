#include "Transform.h"

Transform::Transform()
{
	SetPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetRotation(XMFLOAT3(0.0f, 0.0f, 0.0f));
	SetScale(XMFLOAT3(1.0f, 1.0f, 1.0f));
}

Transform::~Transform()
{
}


void Transform::SetPosition(XMFLOAT3 position) {
	mPosition = position;

}

void Transform::SetRotation(XMFLOAT3 rotation)
{
	mRotation = rotation;
}

void Transform::SetScale(XMFLOAT3 scale)
{
	mScale = scale;
}

void Transform::SetScale(float x, float y, float z)
{
	mScale.x = x;
	mScale.y = y;
	mScale.z = z;

}

void Transform::SetTransform(XMFLOAT4X4 transform)
{
	mTransform = transform;
}

void Transform::SetTransform(XMMATRIX transform)
{
	XMStoreFloat4x4(&mTransform, transform);
}



XMFLOAT4X4 Transform::GetTransform()
{
	return mTransform;
}

XMFLOAT3 Transform::GetPosition()
{
	return mPosition;
}

XMFLOAT3 Transform::GetRotation()
{
	return mRotation;
}

XMFLOAT3 Transform::GetScale()
{
	return mScale;
}

//Apply and update object transforms
XMMATRIX Transform::UpdateTransforms() {

	XMMATRIX transform = XMLoadFloat4x4(&mTransform);
	XMMATRIX scale = XMMatrixScaling(mScale.x, mScale.y, mScale.z);
	XMMATRIX position = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(mRotation.x, mRotation.y, mRotation.z);
	XMMATRIX finalTransform = XMMatrixMultiply(scale, position) * rotation;

	XMStoreFloat4x4(&mTransform, finalTransform);
	return finalTransform;

}
