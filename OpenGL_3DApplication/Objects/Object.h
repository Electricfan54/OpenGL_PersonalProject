#pragma once
#include <GL/glew.h>
#include "../Shaders/Shader.h"
#include "Transform.h"
#include "../Shaders/Material.h"

class Object
{
public:
	struct InstanceData
	{
		Math::mat4f mvp;
		Math::mat4f model;
		Math::mat4f normalMat;
	};

protected:
	GLuint vao;
	GLuint vertBuffer;
	GLuint indexBuffer;
	GLuint indexSize;
	Shader* shader;
	Transform transform;

	material mMaterial;

	Math::vector3 defaultScale;

	// for position calculations
	float width, height;

	// For instancing
	GLuint instanceBuffer;
	unsigned int mInstanceCount = 0;
	
public:

	// This contructor is mainly for allocating memory and does not instantiate a shader
	Object();

	Object(Shader& shader);

	void Bind();
	void Unbind();

	// Instancing
	virtual void SetupInstancing(void* data, unsigned int size, unsigned int instanceCount) = 0;
	virtual void UpdateInstanceBuffer(void* data, unsigned int size, unsigned int instanceCount) = 0;

	Transform& GetTransform()
	{
		return transform;
	}

	virtual void SetPosition(const Math::vector3& pos)
	{
		transform.Position(pos);
	}

	Shader& GetShader()
	{
		return *shader;
	}

	void Color(Math::vector4 newColor) 
	{
		mMaterial.Color(newColor);
	}

	const Math::vector4& Color() const
	{
		return mMaterial.Color();
	}

	const GLuint& IndexSize()const
	{
		return indexSize;
	}

	const material& Material()const 
	{
		return mMaterial;
	}

	material& Material()
	{
		return mMaterial;
	}

	const unsigned int& GetInstanceCount()
	{
		return mInstanceCount;
	}

	Math::mat4f GetModelMatrix() const
	{
		return transform.GetMatrix();
	}

	Math::vector3 GetCenterPosition() const
	{
		Math::vector3 retVec(width, height, -width);
		retVec = Math::vector3(retVec.x * transform.Scale().x, retVec.y * transform.Scale().y, retVec.z * transform.Scale().z);
		retVec = Math::vector3(retVec.x / 2.0f, retVec.y / 2.0f, retVec.z / 2.0f);
		retVec = Math::vector3(retVec.x + transform.Position().x, retVec.y + transform.Position().y, retVec.z + transform.Position().z);
		return  Math::vector3(retVec.x, retVec.y, retVec.z);
	}
};

