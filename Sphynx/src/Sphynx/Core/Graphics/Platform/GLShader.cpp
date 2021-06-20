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
	//std::ostringstream sstr;
	//sstr << in.rdbuf();
	//auto data = sstr.str();
	std::string glsl((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	auto c = glsl.c_str();
	//Creating Shader
	if (id = glCreateShader(SHADER_TYPE)) {
		Core_Error("Unable to create shader object (GL)");
	}
	//Setting the source.
	glShaderSource(id, 1, &c, NULL);
	//Clearing the string since gl copies the code
	//But this means that we did three string copy
	//Two when creating the string and the last being gl.
	//data.clear();
	//sstr.clear();
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
Sphynx::Core::Graphics::GL::GLShader::~GLShader()
{
	glDeleteShader(id);
}
//I'm Stupid. I Have to repeat this Changing only The Shader Type.
void Sphynx::Core::Graphics::GL::GLVertexShader::Load(std::string path)
{
	CreateShader(path, GL_VERTEX_SHADER);
}

void Sphynx::Core::Graphics::GL::GLFragmentShader::Load(std::string path)
{
	CreateShader(path, GL_FRAGMENT_SHADER);
}
