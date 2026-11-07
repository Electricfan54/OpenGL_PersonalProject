#include "Collider.h"

Collider::Collider()
	: extents(0,0,0)
{
}

Collider::Collider(Transform* objectTransform, Math::vector3 _extents)
	: objectTransform(*objectTransform), extents(_extents)
{
	UpdateVerticies();
	UpdateAxes();
}

Collider::~Collider()
{
}

void Collider::UpdateCollider(const Transform& _transform)
{
	objectTransform = _transform;
	UpdateVerticies();
	UpdateAxes();
}

void Collider::UpdatePosition(const Transform& _transform)
{
	objectTransform.Position(_transform.Position());
	UpdateVerticies();
}

void Collider::UpdateVerticies()
{
	verticies.clear();

	float xMin = -extents.x;
	float xMax = extents.x;

	float yMin = -extents.y;
	float yMax = extents.y;

	float zMin = -extents.z;
	float zMax = extents.z;

	verticies.push_back(Math::vector3(xMin, yMin, zMin));
	verticies.push_back(Math::vector3(xMax, yMax, zMax));

	verticies.push_back(Math::vector3(xMin, yMax, zMax));
	verticies.push_back(Math::vector3(xMax, yMin, zMax));
	verticies.push_back(Math::vector3(xMax, yMax, zMin));

	verticies.push_back(Math::vector3(xMin, yMin, zMax));
	verticies.push_back(Math::vector3(xMax, yMin, zMin));
	verticies.push_back(Math::vector3(xMin, yMax, zMin));
	
	verticies.shrink_to_fit();

	// Rotating the verticies
	for (int i = 0; i < verticies.size(); i++)
	{
		Math::quaternion rotatedVertex = objectTransform.Rotation().Conjugate() * Math::quaternion(0, verticies[i].x, verticies[i].y, verticies[i].z) * objectTransform.Rotation();
		verticies[i] = { rotatedVertex.x + centerPos.x, rotatedVertex.y + centerPos.y, rotatedVertex.z + centerPos.z };
	}

}

void Collider::UpdateAxes()
{
	axes.clear();
	axes.push_back(objectTransform.Forward());
	axes.push_back(objectTransform.Right());
	axes.push_back(objectTransform.Up());
	axes.shrink_to_fit();
}

void Collider::SetExtents(const Math::vector3& _extents)
{
	extents = _extents;

}

void Collider::SetCenterPos(const Math::vector3& newPos)
{
	centerPos = newPos;
	UpdateVerticies();
	UpdateAxes();
}
