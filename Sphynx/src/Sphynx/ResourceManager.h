#pragma once
#include "Core/Graphics/Pipeline/Shader.h"
#include "Core/Graphics/Pipeline/Texture.h"
namespace Sphynx {
	class ResourceManager
	{
	private:

	public:
		static Core::Graphics::Shader* LoadShader(const char* path, Core::Graphics::ShaderType type);
		static Core::Graphics::Texture* LoadTexture(const char* path, Core::Graphics::TextureType type, bool Compress);
		//static Core::Graphics::Texture* LoadDepthTexture(const char* path);
	};
}
