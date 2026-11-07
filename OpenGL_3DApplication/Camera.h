#pragma once
#include "Utility/Math.h"

class Camera
{
private:

	Math::vector3 mcamPos;
	Math::quaternion mcamRot;
	Math::vector3 mEulerAngles;

	Math::vector3 mForward;
	Math::vector3 mRight;

	// to get Y rotation for forward vector rotate the vector then project it onto the x,z-Plane
	void UpdateDirs();

public:

	Camera();


	void RotateEulerAngles();

	//Getters
	Math::vector3& Position()
	{
		return mcamPos;
	}

	Math::vector3& EulerAngles()
	{
		return mEulerAngles;
	}

	const Math::vector3& Position() const
	{
		return mcamPos;
	}

	const Math::quaternion& Rotation() const
	{
		return mcamRot;
	}

	const Math::vector3& Forward() const
	{
		return mForward;
	}

	const Math::vector3& Right() const
	{
		return mRight;
	}

	//Setters
	void Position(const Math::vector3& newPos)
	{
		mcamPos = newPos;
	}

	void Rotation(const Math::quaternion& newRot)
	{
		mcamRot = newRot;
		UpdateDirs();
	}

};

