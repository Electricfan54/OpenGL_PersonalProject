#pragma once
#include <vector>
#include <iostream>
#include "Collider.h"
#include "../../Objects/PhysicsObject.h"
#include "../Chunk.h"


class CollisionManager
{
	enum eventType
	{
		enter,
		collision,
		exit,
	};

	struct CollisionEvent
	{
		PhysicsObject* object;
		PhysicsObject* otherObject;
		Math::vector3 mtv;
		eventType type;
	};

	std::vector<PhysicsObject*> staticObjects;

	std::unordered_map<Math::vector3, Chunk*> collisionChunks;
	
	std::vector<PhysicsObject*> dynamicObjects;
	std::vector<CollisionEvent> collisionEvents;

	float minOverlap;
	Math::vector3 minVector;

	bool CheckForCollision(const PhysicsObject* object1, const PhysicsObject* object2, std::vector<Math::vector3>& axes);
	float CalculateOverlap(const Math::vector3& axis, const std::vector<Math::vector3>& verts1, const std::vector<Math::vector3>& verts2) const;
	void CalculateAxes(const Collider* col1, const Collider* col2, std::vector<Math::vector3>& axes) const;

	bool Find(const std::vector<PhysicsObject*>& vec, const PhysicsObject* object) const;
	bool FindAndRemove(std::vector<PhysicsObject*>& vec, const PhysicsObject* object);

	bool HandleObjectCollision(PhysicsObject* dynamicObject, PhysicsObject* staticObject, std::vector<Math::vector3>& axes, Math::vector3& mtv);

public:
	CollisionManager();
	~CollisionManager();


	void HandleCollisions();
	void HandleChunkCollisions();

	void HandleCollisionCallbacks();

	void AddDynamic(PhysicsObject* dynamicObject);
	void AddStatic(PhysicsObject* staticObject);

	void SetCollisionChunks(const std::unordered_map<Math::vector3, Chunk*>& newCollisionChunks)
	{
		collisionChunks = newCollisionChunks;
	}

	static void ThreadEntrypoint(CollisionManager* manager)
	{
		manager->HandleCollisions();
		manager->HandleChunkCollisions();
	}
};