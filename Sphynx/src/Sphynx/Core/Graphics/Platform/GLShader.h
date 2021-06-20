#pragma once
#include "Core/Graphics/Pipeline/Shader.h"

namespace Sphynx::Core::Graphics::GL {
	class GLMaterial;
	class GLShader : public Shader {
	protected:
		unsigned int id;
		void CreateShader(std::string path, int SHADER_TYPE);
		~GLShader();

		friend GLMaterial;
	};
	class GLVertexShader : public GLShader
	{
	private:
		// Inherited via Shader
		virtual void Load(std::string path) override;
	};
	class GLFragmentShader : public GLShader
	{
	private:
		// Inherited via Shader
		virtual void Load(std::string path) override;
	};
}
