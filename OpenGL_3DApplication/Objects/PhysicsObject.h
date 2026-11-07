#pragma once
#include <unordered_map>
#include <functional>
#include "../Utility/Physics/Collider.h"

class PhysicsObject
{
protected:
	Collider collider;
	std::vector<Math::vector3> verticies;

	// TODO:: Change from vector to map
	// Vector for the objects that are currently being collided with
	//std::vector<PhysicsObject*> collidingObjects;
	std::unordered_map<PhysicsObject*, PhysicsObject*> collidingObjects;

	bool isColliding;

	// TODO: Implement checks for this bool
	// Sets whether an object should receive collision events
	bool receiveCollisionEvents;

	// Allows the player to run code when the player ground collider collides with something
	PhysicsObject* OnCollisionEnterEventObject;
	PhysicsObject* OnCollisionEventObject;
	PhysicsObject* OnCollisionExitEventObject;

public:
	PhysicsObject();
	PhysicsObject(Transform* objectTransform);

	virtual void OnCollisionEnter(Math::vector3 mtv, PhysicsObject* other)
	{
		isColliding = true;
		collidingObjects.try_emplace(other, other);
		if (OnCollisionEnterEventObject)
			OnCollisionEnterEventObject->OnCollisionEnter({0,0,0}, nullptr);
	}

	virtual void OnCollision(Math::vector3 mtv, PhysicsObject* other)
	{
		if (OnCollisionEventObject)
			OnCollisionEventObject->OnCollision({ 0,0,0 }, nullptr);
	}

	virtual void OnCollisionExit(PhysicsObject* other)
	{
		isColliding = false;
		collidingObjects.erase(other);
		if (OnCollisionExitEventObject)
			OnCollisionExitEventObject->OnCollisionExit(nullptr);
	}

	const Collider& GetCollider() const;
	Collider& GetCollider();

	const bool& WasColliding() const
	{
		return isColliding;
	}

	const std::unordered_map<PhysicsObject*, PhysicsObject*>& GetCollidingObjects() const
	{
		return collidingObjects;
	}

	const std::vector<Math::vector3>& GetVerticies();

	const bool& GetReceiveCollisionEvents() const
	{
		return receiveCollisionEvents;
	}

	void SetReceiveCollisionEvents(const bool& value)
	{
		receiveCollisionEvents = value;
	}

	void BindOnCollisionEnter(PhysicsObject* objectToBind)
	{
		OnCollisionEnterEventObject = objectToBind;
	}

	void BindOnCollision(PhysicsObject* objectToBind)
	{
		OnCollisionEventObject = objectToBind;
	}

	void BindOnCollisionExit(PhysicsObject* objectToBind)
	{
		OnCollisionExitEventObject = objectToBind;
	}

};

