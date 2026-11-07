#pragma once
#include "../Utility/Math.h"

class Transform
{
private:
	Math::vector3 mPosition;
	Math::quaternion mRotation;
	Math::mat4f mScale;

	Math::vector3 mForward;
	Math::vector3 mRight;
	Math::vector3 mUp;

	void UpdateDirections()
	{
		Math::quaternion newVec = mRotation.Conjugate() * Math::quaternion(0, 0, 0, 1) * mRotation;
		mForward = { newVec.x, newVec.y, newVec.z };
		mForward = mForward.Normalized();

		newVec = mRotation.Conjugate() * Math::quaternion(0, 1, 0, 0) * mRotation;
		mRight = { newVec.x, newVec.y, newVec.z };
		mRight = mRight.Normalized();

		newVec = mRotation.Conjugate() * Math::quaternion(0, 0, 1, 0) * mRotation;
		mUp = { newVec.x, newVec.y, newVec.z };
		mUp = mUp.Normalized();
	}

public:

	void RotateEuler(float x, float y, float z)
	{
		x = x * (3.14f / 180.0f);
		y = y * (3.14f / 180.0f);
		z = z * (3.14f / 180.0f);

		Math::quaternion rotX(cosf(x / 2), sinf(x / 2), 0, 0);
		Math::quaternion rotY(cosf(y / 2), 0, sinf(y / 2), 0);
		Math::quaternion rotZ(cosf(z / 2), 0, 0, sinf(z / 2));
		Math::quaternion rotation = mRotation * rotX * rotY * rotZ;
		Rotation(rotation);
	}

	Transform()
	{
		mScale = Math::IdentityMatrix();
		UpdateDirections();
	}

	void Position(const Math::vector3& pos)
	{
		mPosition = pos;
	}

	Math::vector3& Position()
	{
		return mPosition;
	}

	const Math::vector3& Position()const
	{
		return mPosition;
	}

	void Rotation(const Math::quaternion& rot)
	{
		mRotation = rot;
		UpdateDirections();
	}

	const Math::quaternion& Rotation()const
	{
		return mRotation;
	}

	const Math::vector3 Forward()const
	{
		return mForward;
	}

	const Math::vector3 Right()const
	{
		return mRight;
	}

	const Math::vector3 Up()const
	{
		return mUp;
	}

	void Scale(const float& x, const float& y, const float& z)
	{
		mScale.at(0, 0) = x;
		mScale.at(1, 1) = y;
		mScale.at(2, 2) = z;
	}

	void Scale(Math::vector3 scaleVec)
	{
		mScale.at(0, 0) = scaleVec.x;
		mScale.at(1, 1) = scaleVec.y;
		mScale.at(2, 2) = scaleVec.z;
	}

	Math::vector3 Scale() const
	{
		return { mScale.at(0,0), mScale.at(1,1), mScale.at(2,2) };
	}

	Math::mat4f GetMatrix() const
	{
		return Math::TranslationMatrix(mPosition) * mScale * Math::RotationMatrix(mRotation);
	}

};

