#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <unordered_map>
#include "../Utility/Math.h"


struct shaderText
{
	std::string vertex, fragment;
};

class Shader
{
private:
	unsigned int mId;
	std::unordered_map<std::string, GLuint> mLocationCash;

public:
	Shader(const std::string& fileName);
	~Shader();

	shaderText ParseShader(const std::string& fileName);

	GLuint GetDebugInfo(GLuint id, GLenum shaderType);

	void Bind();
	void Unbind();

	void SetUniform1i(const std::string& name, const int& value);
	void SetUniform1f(const std::string& name, const float& value);
	void SetUniform4f(const std::string& name, const float& x, const float& y, const float& z, const float& w);
	void SetUniform4f(const std::string& name, const Math::vector4& vec);
	void SetUniformMat4f(const std::string& name, const Math::mat4f& matrix, GLenum transpose);

	GLuint GetUniformLocation(const std::string& name);
};