#include "pch.h"
#include "glad/glad.h"
#include "GLShader.h"
#include <sstream>
#include <fstream>
#include <iostream>

void Sphynx::Core::Graphics::GL::GLShader::CreateShader(std::string path, int SHADER_TYPE)
{
	//Read File.
	std::fstream in;
	in.open(path, in.in);
	std::string glsl((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	auto c = glsl.c_str();
	//Creating Shader
	if (!(id = glCreateShader(SHADER_TYPE))) {
		Core_Error("Unable to create shader object (GL)");
	}
	//Setting the source.
	glShaderSource(id, 1, &c, NULL);
	//Clearing the string since gl copies the code
	//But this means that we did three string copy
	//Two when creating the string and the last being gl(i think).
	glsl.clear();
	c = NULL;
	in.close();
	//Compiling Shader.
	glCompileShader(id);

	//	
	GLint compile_status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(id, 1024, &log_length, message);
		Core_Error(message);
	}
}
void Sphynx::Core::Graphics::GL::GLShader::Load(std::string path, ShaderType Type)
{
	int Flag;
	switch (Type)
	{
	case Sphynx::Core::Graphics::ShaderType::VertexShader:
		Flag = GL_VERTEX_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::FragmentShader:
		Flag = GL_FRAGMENT_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::GeometryShader:
		Flag = GL_GEOMETRY_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::TessellationControlShader:
		Flag = GL_TESS_CONTROL_SHADER;
		break; 
	case Sphynx::Core::Graphics::ShaderType::TessellationEvaluationShader:
		Flag = GL_TESS_EVALUATION_SHADER;
		break;
	default:
		Core_Error("Unknown Shader Type. Shader will be Unvalid");
		id = 0;
		return;
	}
	CreateShader(path, Flag);
}

void Sphynx::Core::Graphics::GL::GLShader::Release()noexcept
{
	glDeleteShader(id);
	id = 0;
}

void Sphynx::Core::Graphics::GL::GLShader::CreateFromCode(const char* code, ShaderType Type)
{
	//redundancy

	int Flag;
	switch (Type)
	{
	case Sphynx::Core::Graphics::ShaderType::VertexShader:
		Flag = GL_VERTEX_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::FragmentShader:
		Flag = GL_FRAGMENT_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::GeometryShader:
		Flag = GL_GEOMETRY_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::TessellationControlShader:
		Flag = GL_TESS_CONTROL_SHADER;
		break;
	case Sphynx::Core::Graphics::ShaderType::TessellationEvaluationShader:
		Flag = GL_TESS_EVALUATION_SHADER;
		break;
	default:
		Core_Error("Unknown Shader Type. Shader will be Unvalid");
		id = 0;
		return;
	}
	//GL
	if (!(id = glCreateShader(Flag))) {
		Core_Error("Unable to create shader object (GL)");
	}
	//Setting the source.
	glShaderSource(id, 1, &code, NULL);
	//Compiling Shader.
	glCompileShader(id);
	//Error Handling.
	GLint compile_status;
	glGetShaderiv(id, GL_COMPILE_STATUS, &compile_status);
	if (compile_status != GL_TRUE) {
		GLsizei log_length = 0;
		GLchar message[1024];
		glGetShaderInfoLog(id, 1024, &log_length, message);
		Core_Error(message);
	}
}

Sphynx::Core::Graphics::GL::GLShader::GLShader(GLShader&& shader)noexcept : id(shader.id)
{
}

Sphynx::Core::Graphics::GL::GLShader& Sphynx::Core::Graphics::GL::GLShader::operator=(GLShader&& shader)noexcept
{
	if (this != &shader) {
		this->Release();
		std::swap(id, shader.id);
		std::swap(Type, shader.Type);
	}
	return *this;
}

Sphynx::Core::Graphics::GL::GLShader::~GLShader()
{
	this->Release();
}