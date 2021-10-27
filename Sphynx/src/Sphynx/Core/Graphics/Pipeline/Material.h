#pragma once
#include "Shader.h"
#include "Texture.h"

namespace Sphynx::Core::Graphics {
	class UniformBuffer;
	class Uniform {
		ShaderDataType Type;
	public:
		static Uniform* Create(void* data, ShaderDataType type);
		virtual void SetData(void* data) = 0;
		virtual void Set(const int loc) = 0;
	};
	class Material
	{
	protected:
		Texture* Textures = nullptr;
		virtual void i_CreateMaterial(const ShaderPack& pack) = 0;
		Material(const ShaderPack& shader, Texture* texture) : Textures(texture) {};
	public:
		Material() {};
		//A Material is storage for shaders and textures.
<<<<<<< Updated upstream
		Material& CreateMaterial(const ShaderPack& shaders, Texture* texture) { i_CreateMaterial(shaders); return *this; };
		Texture* GetTextures() { return Textures; };
		virtual bool IsValid() = 0;
		virtual ~Material() = default;
		virtual void Bind() = 0;
=======
		static Material* Create(const ShaderPack& shaders);
		static Material* Create(const ShaderPack& shaders, Texture* textures);
		static Material* Create(const ShaderPack& shaders, std::initializer_list<Texture*> textures);
		static Material* GetDefaultMaterial();
		static Material* GetDefaultMaterialCopy();
		//static void SetUniform();
		virtual bool IsValid() = 0;
		virtual ~Material() = default;
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
		virtual void SetUniform(Uniform* uniform, const char* name) = 0;
		virtual void SetUniform(Uniform* uniform, const int index) = 0;
		virtual void SetUniformBuffer(UniformBuffer* Ubuf, const char* name) = 0;
		virtual void SetUniformBuffer(UniformBuffer* Ubuf, const int Index) = 0;
		virtual void AddTexture(Texture* texture) = 0;
		virtual void RemoveTexture(Texture* texture) = 0;
		virtual void RemoveTexture(unsigned int index) = 0;
		//Clears Textures.
		virtual void RemoveTexture() = 0;
		virtual void SetTexture(Texture* texture, unsigned int index) = 0;
		virtual Texture* GetTexture(unsigned int index) = 0;
		virtual unsigned int GetTextureCount() = 0;
		virtual const unsigned int GetUniformLocation(const char* name) = 0;
		virtual void ReloadShaders(const ShaderPack& pack) = 0;
		virtual Shader* GetDefaultShader(ShaderType type) = 0;
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
	};
}