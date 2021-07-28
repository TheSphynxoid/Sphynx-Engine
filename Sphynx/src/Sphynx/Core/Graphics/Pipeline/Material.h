#pragma once
#include "Shader.h"
#include "Texture.h"

namespace Sphynx::Core::Graphics {
	class Material
	{
	protected:
		Texture* Textures = nullptr;
		virtual void i_CreateMaterial(ShaderPack& pack) = 0;
		Material(ShaderPack& shader, Texture* texture) : Textures(texture) {};
	public:
		Material() {};
		//A Material is stroage for shaders and textures.
		Material& CreateMaterial(ShaderPack& shaders, Texture* texture) { i_CreateMaterial(shaders); return *this; };
		Texture* GetTextures() { return Textures; };
		virtual bool IsValid() = 0;
		virtual ~Material() = default;
		virtual void Bind() = 0;
	};
}