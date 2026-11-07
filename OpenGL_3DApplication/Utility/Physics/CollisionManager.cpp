#include "CollisionManager.h"

CollisionManager::CollisionManager()
	: minOverlap(0)
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::HandleCollisions()
{
	minOverlap = 1000000;

	std::vector<Math::vector3> axes;
	Math::vector3 mtv;
	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		axes = dynamicObjects[i]->GetCollider().GetAxes();
		mtv = { 0,0,0 };
		for (int j = 0; j < staticObjects.size(); j++)
		{
			HandleObjectCollision(dynamicObjects[i], staticObjects[j], axes, mtv);
		}
	}
}

void CollisionManager::HandleChunkCollisions()
{
	minOverlap = 1000000;
	std::vector<Math::vector3> axes;
	Math::vector3 mtv;

	for (int i = 0; i < dynamicObjects.size(); i++)
	{
		axes = dynamicObjects[i]->GetCollider().GetAxes();
		mtv = { 0,0,0 };

		for (auto itr = collisionChunks.begin(); itr != collisionChunks.end(); itr++)
		{
			for (int j = 0; j < itr->second->colliderVector.size(); j++)
			{
				if (HandleObjectCollision(dynamicObjects[i], &itr->second->colliderVector[j], axes, mtv))
				{
					//itr->second->DebugChangeColor();
				}
			}
		}
	}
}

void CollisionManager::HandleCollisionCallbacks()
{
	// Handles all of the collision callbacks generated through the collision thread
	for (int i = 0; i < collisionEvents.size(); i++)
	{
		CollisionEvent& event = collisionEvents[i];
		switch (event.type)
		{
		case eventType::enter:
			event.object->OnCollisionEnter(event.mtv, event.otherObject);
			break;
		case eventType::collision:
			event.object->OnCollision(event.mtv, event.otherObject);
			break;
		case eventType::exit:
			event.object->OnCollisionExit(event.otherObject);
			break;
		}
	}
	collisionEvents.clear();
}

bool CollisionManager::CheckForCollision(const PhysicsObject* object1, const PhysicsObject* object2, std::vector<Math::vector3>& axes)
{
	const Collider* col1 = &object1->GetCollider();
	const Collider* col2 = &object2->GetCollider();

	if (col1 == nullptr || col2 == nullptr)
		return false;

	float debugX = col2->CenterPos().x;
	// Will make huge colliders not work
	if ((col1->CenterPos() - col2->CenterPos()).MagnitudeSquared() > 50.0f)
		return false;

	//CalculateAxes(col1, col2, axes);

	const std::vector<Math::vector3>& axes1 = col1->GetAxes();
	const std::vector<Math::vector3>& axes2 = col2->GetAxes();

	axes = axes1;
	for (int i = 0; i < axes2.size(); i++)
	{
		axes.push_back(axes2[i]);
	}

	// Checking only 6 axes before computing the other 9
	for (int i = 0; i < axes.size(); i++)
	{
		float overlap = CalculateOverlap(axes[i], col1->GetVerticies(), col2->GetVerticies());
		if (overlap == 0.0f)// Not colliding
			return false;

		if (overlap < 0.0f)
		{
			std::cout << "Collision overlap is negative" << std::endl;
		}

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minVector = axes[i];
		}
	}

	int offset = axes.size();
	// Calculating the cross products
	for (int i = 0; i < axes1.size(); i++)
	{
		for (int j = 0; j < axes2.size(); j++)
		{
			Math::vector3 crossResult = Math::vector3::Cross(axes1[i], axes2[j]);
			if (crossResult == Math::vector3(0, 0, 0))
				continue;
			axes.push_back(crossResult);
		}
	}

	// Checking the other 9 axes
	for (int i = offset; i < axes.size(); i++)
	{
		float overlap = CalculateOverlap(axes[i], col1->GetVerticies(), col2->GetVerticies());
		if (overlap == 0.0f)// Not colliding
			return false;

		if (overlap < 0.0f)
		{
			std::cout << "Collision overlap is negative" << std::endl;
		}

		if (overlap < minOverlap)
		{
			minOverlap = overlap;
			minVector = axes[i];
		}
	}

	// Colliding
	return true;
}

float CollisionManager::CalculateOverlap(const Math::vector3& axis, const std::vector<Math::vector3>& verts1, const std::vector<Math::vector3>& verts2) const
{
	if (verts1.size() == 0 || verts2.size() == 0)
		return 0.0f;

	// Projecting verts1 onto axis
	float result = verts1[0] * axis;
	float min1 = result;
	float max1 = result;

	for (int i = 1; i < verts1.size(); i++)
	{
		result = verts1[i] * axis;
		if (result < min1)
			min1 = result;
		else if (result > max1)
			max1 = result;
	}

	// Projecting verts2 onto axis
	result = verts2[0] * axis;
	float min2 = result;
	float max2 = result;

	for (int i = 1; i < verts2.size(); i++)
	{
		result = verts2[i] * axis;
		if (result < min2)
			min2 = result;
		else if (result > max2)
			max2 = result;
	}

	// Checking for overlap
	// Can be cleaned up
	if (min1 > min2 && min1 < max2)
	{
		// overlapping
		return max2 - min1;
	}
	else if (max1 > min2 && max1 < max2)
	{
		// overlapping
		return max1 - min2;
	}
	else if (min1 == min2 && max1 < max2)
	{
		// overlapping
		return max1 - min1;
	}
	else if (min1 == min2 && max2 < max1)
	{
		// overlapping
		return max2 - min2;
	}
	else if (max1 == max2 && min1 < min2)
	{
		// overlapping
		return max1 - min1;
	}
	else if (max1 == max2 && min2 < min1)
	{
		// overlapping
		return max2 - min2;
	}
	else if (min1 > min2 && max1 < max2)
	{
		// min1 and max1 are contained between min2 and max2
		// The overlap value may cause bugs
		return max1 - min1;
	}
	else if (min2 > min1 && max2 < max1)
	{
		// min2 and max2 are contained between min1 and max1
		// The overlap value may cause bugs
		return max2 - min2;
	}
	else
		return 0.0f; // Not overlapping


	return 0.0f;
}

void CollisionManager::CalculateAxes(const Collider* col1, const Collider* col2, std::vector<Math::vector3>& axes) const
{
	const std::vector<Math::vector3>& axes1 = col1->GetAxes();
	const std::vector<Math::vector3>& axes2 = col2->GetAxes();

	// Can optimize by only calculating axes2 and pre calculating axes1
	axes = axes1;
	// Cubes have 15 seperating axes. 3 for each cube and 9 for the cross prodructs of the cube axes
	axes.reserve(12);
	for (int i = 0; i < axes2.size(); i++)
	{
		axes.push_back(axes2[i]);

		// calculating cross product axes
		for (int j = 0; j < axes1.size(); j++)
		{
			Math::vector3 crossResult = Math::vector3::Cross(axes2[i], axes1[j]);
			if (crossResult == Math::vector3(0, 0, 0))
				continue;
			axes.push_back(crossResult);
		}

	}
}

bool CollisionManager::Find(const std::vector<PhysicsObject*>& vec, const PhysicsObject* object) const
{
	for (int i = 0; i < vec.size(); i++)
	{
		if (vec[i] == object)
		{
			return true;
		}
	}
	return false;
}

bool CollisionManager::FindAndRemove(std::vector<PhysicsObject*>& vec, const PhysicsObject* object)
{
	for (std::vector<PhysicsObject*>::iterator iter = vec.begin(); iter != vec.end(); iter++)
	{
		if (*iter == object)
		{
			vec.erase(iter);
			return true;
		}
	}
	return false;
}

bool CollisionManager::HandleObjectCollision(PhysicsObject* dynamicObject, PhysicsObject* staticObject, std::vector<Math::vector3>& axes, Math::vector3& mtv)
{
	if (!CheckForCollision(dynamicObject, staticObject, axes))
	{
		// TODO: Optimize
		// Temp code for testing
		if (dynamicObject->GetCollidingObjects().contains(staticObject))
		{
			CollisionEvent event;
			event.mtv = { 0,0,0 };
			event.object = dynamicObject;
			event.otherObject = staticObject;
			event.type = eventType::exit;
			collisionEvents.push_back(event);
		}

		if (staticObject->GetCollidingObjects().contains(dynamicObject))
		{
			CollisionEvent event;
			event.mtv = { 0,0,0 };
			event.object = staticObject;
			event.otherObject = dynamicObject;
			event.type = eventType::exit;
			collisionEvents.push_back(event);
		}

		return false;
	}

	Math::vector3 pos1 = dynamicObject->GetCollider().CenterPos();
	Math::vector3 pos2 = staticObject->GetCollider().CenterPos();

	Math::vector3 mtvDir = pos2 - pos1;

	if (mtvDir.Normalized() * minVector > 0)
		minVector = minVector * -1;

	mtv = minVector * minOverlap;

	if (dynamicObject->GetReceiveCollisionEvents())
	{
		// Create collision event for the dynamic object
		CollisionEvent event;
		event.mtv = mtv;
		event.object = dynamicObject;
		event.otherObject = staticObject;
		if (dynamicObject->GetCollidingObjects().contains(staticObject))
			event.type = eventType::collision;
		else
			event.type = eventType::enter;

		collisionEvents.push_back(event);
	}

	if (staticObject->GetReceiveCollisionEvents())
	{
		// Create collision event for the dynamic object
		CollisionEvent event;
		event.mtv = mtv;
		event.object = staticObject;
		event.otherObject = dynamicObject;
		if (staticObject->GetCollidingObjects().contains(dynamicObject))
			event.type = eventType::collision;
		else
			event.type = eventType::enter;

		collisionEvents.push_back(event);
	}
	return true;
}

void CollisionManager::AddDynamic(PhysicsObject* dynamicObject)
{
	dynamicObjects.push_back(dynamicObject);
}

void CollisionManager::AddStatic(PhysicsObject* staticObject)
{
	staticObjects.push_back(staticObject);
}
