#pragma once
#include"Core/Graphics/Pipeline/Shader.h"

namespace Sphynx::Core::Graphics::GL {
	class GLShader : public Shader {
	protected:
		//Shader Object ID
		unsigned int id = 0;
		//Handles OpenGl Shader Creation, Compiles The Code from the given file and 
		//creates the shader object of which the id is stored
		void CreateShader(std::string path, int SHADER_TYPE);
		//Inherited From Shader, Gets Called To Create Shader.
		void Load(std::string path, ShaderType type);
		void Release()noexcept;
	public:
		void CreateFromCode(const char* code, ShaderType Type);
		GLShader() = default;
		//Move-Only
		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;
		//Move Constructor
		GLShader(GLShader&& shader)noexcept;
		GLShader& operator=(GLShader&& shader)noexcept;

		virtual bool IsValid() override { return id; };
		//Destructor
		~GLShader();
		friend class GLMaterial;
		friend class GLRenderer;
	};
}