#pragma once
#include "stb_truetype.h"

namespace Sphynx::Core {
	namespace Graphics {
		class Texture;
	}
	class Font
	{
	private:
		const unsigned char* Data;
		stbtt_fontinfo FontInfo;
		Graphics::Texture* FontBitmap = nullptr;
		const char* FontName;
		int FontHeight = 34;
	public:
		Font(const unsigned char* data);
		~Font();
		void SetFontSize(int height);
		int GetFontSize() { return FontHeight; };
		const char* GetFontName() { return FontName; };
		Graphics::Texture* GetFontBitmap() { return FontBitmap; };
	};
}
