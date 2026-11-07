#include "Object.h"

Object::Object()
	: indexBuffer(0), vertBuffer(0), vao(0), indexSize(0), instanceBuffer(0), defaultScale(1.0f, 1.0f, 1.0f), width(0.0f), height(0.0f)
{
}

Object::Object(Shader& shader)
	: indexBuffer(0), vertBuffer(0), vao(0), indexSize(0), instanceBuffer(0), defaultScale(1.0f, 1.0f, 1.0f), width(0.0f), height(0.0f)
{
	this->shader = &shader;
}

void Object::Bind()
{
	glBindVertexArray(vao);
	shader->Bind();
	if (mInstanceCount > 0)
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, instanceBuffer);
}

void Object::Unbind()
{
	glBindVertexArray(0);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, 0);
}