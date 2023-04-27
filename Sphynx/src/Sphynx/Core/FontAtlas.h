#pragma once
#include "Font.h"

namespace Sphynx::Core {
	namespace Graphics {
		class Texture;
	}
	class FontAtlas {
	private:
		std::vector<Font> Fonts;
		Graphics::Texture* Atlas = nullptr;
		Graphics::Texture* AllocateTexture();
	public:
		FontAtlas();
		void AddFont(Font* font) { Fonts.push_back(SPH_Forward(*font)); }
		void PrepareAtlas();
		inline Graphics::Texture* GetAtlasTexture() { return Atlas; }
	};
}