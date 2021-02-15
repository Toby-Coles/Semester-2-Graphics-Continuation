#include "Transform.h"

Transform::Transform()
{
	//mPosition = new Vector();
	//mRotation = new Vector();
	//mScale = new Vector();

	mPosition = Vector(0, 0, 0);
	mScale = Vector(0, 0, 0);
		mRotation = Vector(0, 0, 0);

	SetScale(1.0f, 1.0f, 1.0f);
	UpdateTransforms();
}

Transform::~Transform()
{
}


void Transform::SetPosition(Vector position) {
	mPosition = position;

}

void Transform::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}

void Transform::SetRotation(Vector rotation)
{
	mRotation = rotation;
}
void Transform::SetRotation(float x, float y, float z) {

	mRotation.x = x;

	mRotation.y = y;

	mRotation.z = z;
	
}

void Transform::SetScale(Vector scale)
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

Vector* Transform::GetPosition()
{
	return &mPosition;
}

Vector* Transform::GetRotation()
{
	return &mRotation;
}

Vector* Transform::GetScale()
{
	return &mScale;
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
