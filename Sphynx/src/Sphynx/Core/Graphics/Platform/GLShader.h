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
		//Inherited From Shader, Gets Called To Create Shader.
		void Release()noexcept;
	public:
		GLShader(const char* code, ShaderType Type);
		//Spec:
		//location 0 :Will always be for Vertex Position,
		//Custom Attributes Should begin at 4.
		GLShader() = default;
		//Move-Only
		GLShader(const GLShader&) = delete;
		GLShader& operator=(const GLShader&) = delete;
		//Move Constructor
		GLShader(GLShader&& shader)noexcept;
		GLShader& operator=(GLShader&& shader)noexcept;
		virtual bool IsValid() noexcept override { return id; };
		//Not a pointer.
		virtual void* GetNative() noexcept { return (void*)id; }
		//Destructor
		~GLShader();
		friend class GLMaterial;
		friend class GLRenderer;
	};
}