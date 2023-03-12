#pragma once
#include "Core/Graphics/Imgui.h"
#include <map>
#include "Core/Graphics/Pipeline/Texture.h"
#include "ResourceManager.h"

namespace Sphynx::Core {
	class TextureDebugger : public Sphynx::Core::IOverlayWindow
	{
	private:
		std::map<const char*, ResourceManager::TexResource> loadedTex;
		static inline std::vector<Graphics::Texture*> SpecialTex;
	public:
		TextureDebugger() {
			loadedTex = ResourceManager::GetLoadedTextures();
		}
		~TextureDebugger() {

		}
		static void AddTexture(Graphics::Texture* tex) {
			SpecialTex.push_back(tex);
		}
		// Inherited via IOverlayWindow
		virtual void Draw() override;
	};
}
