#pragma once
#include "Object.h"

class Plane : public Object
{
private:

public:
	Plane(Shader& shader);

	void SetupInstancing(void* data, unsigned int size, unsigned int instanceCount) override;
	void UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount) override;
};

