#pragma once
#include <vector>
#include "../Math.h"
#include "../../Objects//Transform.h"

class Collider
{
private:
	std::vector<Math::vector3> axes;
	std::vector<Math::vector3> verticies;
	Math::vector3 extents;
	Transform objectTransform;
	Math::vector3 centerPos;

	void UpdateVerticies();
	void UpdateAxes();
public:

	Collider();
	Collider(Transform* objectTransform, Math::vector3 _extents = Math::vector3(.5, .5, .5));

	~Collider();

	void UpdateCollider(const Transform& _transform);
	void UpdatePosition(const Transform& _transform);

	const std::vector<Math::vector3>& GetAxes() const
	{
		return axes;
	}

	const std::vector<Math::vector3>& GetVerticies() const
	{
		return verticies;
	}

	const Math::vector3& CenterPos() const
	{
		return centerPos;
	}

	const Transform* GetTransform() const
	{
		return &objectTransform;
	}

	void SetExtents(const Math::vector3& _extents);

	void SetTransform(const Transform* newTransform)
	{
		objectTransform = (*newTransform);
		UpdateAxes();
		//UpdateVerticies();
	}

	void SetCenterPos(const Math::vector3& newPos);
};

