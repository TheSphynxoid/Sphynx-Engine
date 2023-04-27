#pragma once

struct stbtt_fontinfo;
//struct stbtt_packedchar;

namespace Sphynx::Core {
	namespace Graphics {
		class Texture;
	};
	class FontAtlas;
	//Written on top of stbtt_truefont.
	class Font
	{
	private:
		const unsigned char* Data;
		stbtt_fontinfo* FontInfo;
		//stbtt_packedchar* PackedChars;
		std::string FontName;
		int FontHeight;
		int Ascent, Descent, LineGap, BaseLine;
		float Scaling;
		FontAtlas* Atlas;
	public:
		Font(std::string name, const unsigned char* data, int Height = 34);
		~Font();
		void SetFontSize(int height);
		int GetFontSize() { return FontHeight; };
		Graphics::Texture* GetCharacterBitmap(char c);
		std::string GetFontName() { return FontName; };
		inline FontAtlas* GetAtlas() { return Atlas; };
		static wchar_t* GetLatinRange() {
			wchar_t range[] = {
				0x0020, 0x00FF,		// Basic Latin + Latin Supplement
				//0x2000, 0x206F,		//General Ponctuation.
				0
			};
			return range;
		};
		friend FontAtlas;
	};
}
