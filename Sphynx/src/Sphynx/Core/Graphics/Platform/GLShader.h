#pragma once
#include"Core/Graphics/Pipeline/Shader.h"
#include "Vector.h"
namespace Sphynx::Core::Graphics::GL {
	class GLShader : public Shader {
	protected:
		//Shader Object ID
		unsigned int id = 0;
		inline static GLShader* DefaultVertexShader = nullptr;
		inline static GLShader* DefaultFragmentShader = nullptr;
		std::string ReadFile(std::string path);
		//Handles OpenGl Shader Creation, Compiles The Code from the given file and 
		//creates the shader object of which the id is stored
		void Create(std::string path, int SHADER_TYPE);
		//Inherited From Shader, Gets Called To Create Shader.
		void Release()noexcept;
	public:
		GLShader(std::string path, ShaderType type);
		GLShader(const char* code, ShaderType Type);
		//Spec:
		//location 0 :Will always be for Vertex Position,
		//location 1 :Will be for clip matrix.
		//location 2 :Will be for textures if they are present.
		//location 3 :Will Be for normal.
		//Custom Attributes Should begin at 4.
		GLShader() = default;
		//Move-Only
		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;
		//Move Constructor
		GLShader(GLShader&& shader)noexcept;
		GLShader& operator=(GLShader&& shader)noexcept;
		virtual bool IsValid() noexcept override { return id; };
		//Destructor
		~GLShader();
		friend class GLMaterial;
		friend class GLRenderer;
	};
}