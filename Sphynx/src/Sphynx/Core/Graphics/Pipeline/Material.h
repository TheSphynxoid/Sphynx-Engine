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
		Material(const ShaderPack& shader, Texture* texture) {};
	public:
		Material() = default;
		//A Material is storage for shaders and textures.
		static Material* Create(const ShaderPack& shaders);
		static Material* Create(const ShaderPack& shaders, Texture* textures);
		static Material* Create(const ShaderPack& shaders, std::initializer_list<Texture*> textures);
		static Material* GetDefaultMaterial();
		static Material* GetDefaultMaterialCopy();
		static Shader* GetDefaultShader(ShaderType type);
		//static void SetUniform();
		virtual bool IsValid()const noexcept = 0;
		virtual ~Material() = default;
		virtual void Bind()noexcept = 0;
		virtual void Unbind()noexcept = 0;
		virtual void SetUniform(Uniform* uniform, const char* name) = 0;
		virtual void SetUniform(Uniform* uniform, const int index) = 0;
		virtual void SetUniformBuffer(UniformBuffer* Ubuf, const char* name) = 0;
		virtual void SetUniformBuffer(UniformBuffer* Ubuf, const int Index) = 0;
		virtual void AddTexture(Texture* texture) = 0;
		virtual void RemoveTexture(Texture* texture) = 0;
		virtual void RemoveTexture(unsigned int index) = 0;
		//Clears Textures.
		virtual void ClearTextures() = 0;
		virtual void SetTexture(Texture* texture, unsigned int index) = 0;
		virtual Texture* GetTexture(unsigned int index) = 0;
		virtual unsigned int GetTextureCount()const noexcept = 0;
		virtual const int GetUniformLocation(const char* name) = 0;
		virtual void ReloadShaders(const ShaderPack& pack) = 0;
		virtual void ReloadShaders(Shader* shader) = 0;
		virtual const ShaderPack& GetShaders()const = 0;
	};
}