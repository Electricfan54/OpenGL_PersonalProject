#pragma once
#include "Objects/Object.h"
class Renderer
{
private:
	Math::mat4f viewMatrix;
	int width, height;

	Math::mat4f projMatrix;

	Math::vector3 lightPos;
	Math::vector4 lightColor;

public:
	Renderer();

	void SetViewMatrix(const Math::mat4f& matrix);
	void SetWindowSize(int w, int h);

	void RenderObject(Object& object);
	void RenderObjectInstanced(Object& object);
	void RenderSkybox(Object& object, GLuint& textureid);
	
	void SetLightPos(const Math::vector3& pos);
	void SetLightColor(const Math::vector4& color);
};

