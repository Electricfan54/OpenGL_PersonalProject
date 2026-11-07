#pragma once
#include "Object.h"

class Skybox : public Object
{
private:
	std::vector<float> positions;
	std::vector<std::string> cubemapTextures;

	void LoadDataToGPU();
public:

	GLuint textureId;

	Skybox(Shader& shader);
	Skybox(Shader& shader, const std::vector<std::string>& cubemapTextures);

	// Instancing overrides
	void SetupInstancing(void* data, unsigned int size, unsigned int instanceCount) override;

	void UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount) override;
};

