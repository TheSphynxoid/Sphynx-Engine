#pragma once
#include "Core/Graphics/Pipeline/Shader.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Core/Font.h"
#include <map>

namespace Sphynx {
	class GameObject;
	namespace Core {
		class Font;
	}
	class ResourceManager
	{
	public:
		struct TexResource {
			Core::Graphics::Texture* _tex = nullptr;
			bool Compressed;
		};
	private:
		inline static std::map<const char*, TexResource> OpenTex;
		inline static std::map<const char*, Core::Font*> LoadedFonts;
	public:
		//Returns a map of all the loaded textures and their paths.
		static std::map<const char*, TexResource> GetLoadedTextures() { return OpenTex; };
		//Loads a shader from file.
		static Core::Graphics::Shader* LoadShader(const char* path, Core::Graphics::ShaderType type);
		//Loads a texture from file.
		static Core::Graphics::Texture* LoadTexture(const char* path, Core::Graphics::TextureType type, bool Compress = false);
		//Loads a font file.
		static Core::Font* LoadFont(const char* path);
		//Releases all resources.
		static void ReleaseResources();
		//Releases all fonts.
		static void ReleaseFonts();
		//Releases all textures
		static void ReleaseTextures();
		//Loads a script component into a Gameobject.
		static void LoadScript(const char* path, GameObject* GO);
		//static Core::Graphics::Texture* LoadDepthTexture(const char* path);
	};
}
