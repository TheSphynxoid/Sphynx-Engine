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
		Material(ShaderPack shaders, Texture* texture) :Shaders(Shaders), Textures(texture) {};
		virtual ~Material() = 0;
		virtual void Bind() = 0;
	};
}