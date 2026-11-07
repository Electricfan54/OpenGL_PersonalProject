#include "Camera.h"

void Camera::UpdateDirs()
{
	Math::quaternion newVec = mcamRot.Conjugate() * Math::quaternion(0, 0, 0, 1) * mcamRot;
	mForward = { newVec.x, 0, newVec.z };
	mForward = mForward.Normalized();

	newVec = mcamRot.Conjugate() * Math::quaternion(0, 1, 0, 0) * mcamRot;
	mRight = { newVec.x, 0, newVec.z };
	mRight = mRight.Normalized();
}

Camera::Camera()
{
	mForward = { 0,0,1 };
	mRight = {1, 0, 0};
}

void Camera::RotateEulerAngles()
{
	float theta  = mEulerAngles.x * (3.14f / 180.0f);
	float theta2 = mEulerAngles.y * (3.14f / 180.0f);
	Math::quaternion rotation = Math::quaternion(cosf(theta / 2), sinf(theta / 2), 0, 0) * Math::quaternion(cosf(theta2 / 2), 0, sinf(theta2 / 2), 0);
	Rotation(rotation);
}
