#include "PhysicsObject.h"

PhysicsObject::PhysicsObject()
    : isColliding(false)
{
}

PhysicsObject::PhysicsObject(Transform* objectTransform)
    : collider(objectTransform), isColliding(false)
{
}

const Collider& PhysicsObject::GetCollider() const
{
    return collider;
}

Collider& PhysicsObject::GetCollider()
{
    return collider;
}

const std::vector<Math::vector3>& PhysicsObject::GetVerticies()
{
    return verticies;
}
