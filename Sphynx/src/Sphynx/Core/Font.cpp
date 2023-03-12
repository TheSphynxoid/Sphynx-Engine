#include "pch.h"
#include "Font.h"
#include "Core/Graphics/Pipeline/Texture.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
#include "stb_textedit.h"

using namespace Sphynx::Core::Graphics;

Sphynx::Core::Font::Font(const unsigned char* data, int Height) : Data(data), FontHeight(Height)
{
	//Load Font
	FontInfo = new stbtt_fontinfo();
	stbtt_InitFont(FontInfo, data, 0);
	Scaling = stbtt_ScaleForPixelHeight(FontInfo, FontHeight);

	//Getting Metrics.
	stbtt_GetFontVMetrics(FontInfo, &Ascent, &Descent, 0);
	BaseLine = Ascent * Descent;
	
}

Sphynx::Core::Font::~Font()
{
	delete FontInfo;
	free((void*)Data);
}

void Sphynx::Core::Font::SetFontSize(int height)
{
	FontHeight = height;
}

Sphynx::Core::Graphics::Texture* Sphynx::Core::Font::GetCharacterBitmap(char c)
{
	int Advance, LeftBearing;
	stbtt_GetCodepointHMetrics(FontInfo, c, &Advance, &LeftBearing);
	int TexWidth = Advance * Scaling;
	//Create Texture Object
	auto tex = Texture::Create(TextureType::Texture2D, Advance * Scaling, (Ascent - Descent) * Scaling, TextureFormat::Red, TextureDataFormat::Byte);
	return nullptr;
}
