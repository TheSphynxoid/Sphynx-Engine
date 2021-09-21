#pragma once
#include "Shader.h"
#include "Texture.h"

namespace Sphynx::Core::Graphics {
	class Material
	{
	protected:
		Texture* Textures = nullptr;
		virtual void i_CreateMaterial(const ShaderPack& pack) = 0;
		Material(const ShaderPack& shader, Texture* texture) : Textures(texture) {};
	public:
		Material() {};
		//A Material is storage for shaders and textures.
		Material& CreateMaterial(const ShaderPack& shaders, Texture* texture) { i_CreateMaterial(shaders); return *this; };
		Texture* GetTextures() { return Textures; };
		virtual bool IsValid() = 0;
		virtual ~Material() = default;
		virtual void Bind() = 0;
	};
}