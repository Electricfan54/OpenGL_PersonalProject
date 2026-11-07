#include "Shader.h"

Shader::Shader(const std::string& fileName)
	: mId(0)
{
	shaderText shader = ParseShader(fileName);

	unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* source = shader.vertex.c_str();
	glShaderSource(vs, 1, &source, nullptr);
	glCompileShader(vs);

	GetDebugInfo(vs, GL_VERTEX_SHADER);


	unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* source2 = shader.fragment.c_str();
	glShaderSource(fs, 1, &source2, nullptr);
	glCompileShader(fs);

	GetDebugInfo(fs, GL_FRAGMENT_SHADER);

	mId = glCreateProgram();
	
	glAttachShader(mId, vs);
	glAttachShader(mId, fs);

	glLinkProgram(mId);
	GetDebugInfo(mId, GL_PROGRAM);
	
	
	glDetachShader(mId, vs);
	glDetachShader(mId, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

Shader::~Shader()
{
}

shaderText Shader::ParseShader(const std::string& fileName)
{
	// #shader vertex/fragment designates the shader sections
	std::ifstream file;
	file.open(fileName);

	if (!file.is_open())
	{
		std::cout << "Cannot open shader\n";
		file.close();
		return shaderText{};
	}

	std::stringstream ss[2];
	std::string line;
	int i = 0;
	while (std::getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				i = 0;
				continue;
			}
			if (line.find("fragment") != std::string::npos)
			{
				i = 1;
				continue;
			}
		}
		ss[i] << line << "\n";
	}

	file.close();
	return shaderText{ss[0].str(), ss[1].str()};
}

GLuint Shader::GetDebugInfo(GLuint id, GLenum shaderType)
{

	if (shaderType == GL_PROGRAM)
	{
		GLint isLinked = 0;
		glGetProgramiv(mId, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(mId, GL_INFO_LOG_LENGTH, &length);
			std::vector<GLchar> infoLog(length);
			glGetProgramInfoLog(mId, length, &length, &infoLog[0]);
			glDeleteProgram(mId);

			std::cout << "Shader link error: ";
			for (GLchar& character : infoLog)
			{
				std::cout << character;
			}
			std::cout << std::endl;
			return 0;
		}
	}

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (shaderType == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader\n" << message << std::endl;
		glDeleteShader(id);
		delete[] message;
		message = nullptr;
		return 0;
	}
	return id;
}

void Shader::Bind()
{
	glUseProgram(mId);
}

void Shader::Unbind()
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, const int& value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, const float& value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform4f(const std::string& name, const float& x, const float& y, const float& z, const float& w)
{
	glUniform4f(GetUniformLocation(name), x, y, z, w);
}

void Shader::SetUniform4f(const std::string& name, const Math::vector4& vec)
{
	glUniform4f(GetUniformLocation(name), vec.x, vec.y, vec.z, vec.w);
}

void Shader::SetUniformMat4f(const std::string& name, const Math::mat4f& matrix, GLenum transpose)
{
	glUniformMatrix4fv(GetUniformLocation(name), 1, transpose, matrix.matrix);
}

GLuint Shader::GetUniformLocation(const std::string& name)
{
	if (mLocationCash.find(name) != mLocationCash.end())
		return mLocationCash[name];

	GLuint location = glGetUniformLocation(mId, name.c_str());
	mLocationCash[name] = location;
	return location;
}
