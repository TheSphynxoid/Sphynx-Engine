#pragma once
#include "Shader.h"
#include "Texture.h"

namespace Sphynx::Core::Graphics {
	class Material
	{
	protected:
		ShaderPack Shaders;
		Texture* Textures;
	public:
		//A Material is a holder for shaders and textures.
		Material(ShaderPack shaders, Texture* texture) :Shaders(Shaders), Textures(texture) {};
		ShaderPack GetShaders() { return Shaders; };
		Texture* GetTextures() { return Textures; };
		virtual ~Material() = default;
		virtual void Bind() = 0;
	};
}