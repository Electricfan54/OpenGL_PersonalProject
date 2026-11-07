#pragma once
#include <vector>
#include "Object.h"
#include "../Utility/Math.h"
#include "PhysicsObject.h"
#include "../Utility/Physics/Collider.h"

class Cube : public Object, public PhysicsObject
{
private:

	std::vector<float> positions;

	// For debuging collision code
	Math::vector4 prevColor;
public:

	Cube();
	Cube(Shader& shader);

	void SetPosition(const Math::vector3& pos) override;
	
	// Instancing overrides
	void SetupInstancing(void* data, unsigned int size, unsigned int instanceCount) override;

	void UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount) override;

	void OnCollisionEnter(Math::vector3 mtv, PhysicsObject* other) override;
	void OnCollision(Math::vector3 mtv, PhysicsObject* other) override;
	void OnCollisionExit(PhysicsObject* other) override;
};

