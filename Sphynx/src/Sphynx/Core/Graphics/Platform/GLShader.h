#pragma once
#include"Core/Graphics/Pipeline/Shader.h"

namespace Sphynx::Core::Graphics::GL {
	//class GLMaterial;

	class GLShader : public Shader {
	protected:
		//Shader Object ID
		unsigned int id;
		//Handles OpenGl Shader Creation, Compiles The Code from the given file and 
		//creates the shader object of which the id is stored
		void CreateShader(std::string path, int SHADER_TYPE);
		//Inherited From Shader, Gets Called To Create Shader.
		void Load(std::string path, ShaderType type);
	public:
		void CreateFromCode(const char* code, ShaderType Type);
		//Destructor
		~GLShader();
		friend class GLMaterial;
		friend class GLRenderer;
	};
}
