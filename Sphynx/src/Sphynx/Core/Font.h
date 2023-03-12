#pragma once

struct stbtt_fontinfo;

namespace Sphynx::Core {
	namespace Graphics {
		class Texture;
	}
	class Font
	{
	private:
		const unsigned char* Data;
		stbtt_fontinfo* FontInfo;
		const char* FontName;
		int FontHeight;
		int Ascent, Descent, BaseLine;
		float Scaling;
	public:
		Font(const unsigned char* data, int Height = 34);
		~Font();
		void SetFontSize(int height);
		int GetFontSize() { return FontHeight; };
		Graphics::Texture* GetCharacterBitmap(char c);
		const char* GetFontName() { return FontName; };
	};
}
