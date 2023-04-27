#include "pch.h"
#include "Font.h"
#include "Core/Thread.h"
#include "Core/Graphics/Pipeline/Texture.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"
#include "stb_image_write.h"
#include <filesystem>

using namespace Sphynx::Core::Graphics;

Sphynx::Core::Font::Font(std::string name, const unsigned char* data, int Height) : Data(data), FontHeight(Height), FontName(name)
{
	//Load Font
	FontInfo = new stbtt_fontinfo();
	if (!stbtt_InitFont(FontInfo, data, 0)) {
		this->~Font();
		return;
	}

	Scaling = stbtt_ScaleForPixelHeight(FontInfo, FontHeight);

	//Getting Metrics.
	stbtt_GetFontVMetrics(FontInfo, &Ascent, &Descent, &LineGap);
	BaseLine = Ascent * Descent/* *Scaling*/;
	free((void*)data);
}

Sphynx::Core::Font::~Font()
{
	delete FontInfo;
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
	int W, H, OX, OY;
	void* Bm = (void*)stbtt_GetCodepointBitmap(FontInfo, 0, Scaling, c, &W, &H, &OX, &OY);
	//stbtt_GetCodepointBitmap(FontInfo,)
	//Create Texture Object
	auto tex = Texture::Create(Bm, W, H, TextureType::Texture2D, TextureFormat::Red, TextureDataFormat::UByte);
	return tex;
}

//Sphynx::Core::Graphics::Texture* Sphynx::Core::Font::CreateFontAtlas()
//{
//	stbtt_pack_context pcontext;
//#ifdef DEBUG
//	constexpr size_t atlas = 512;
//#endif
//	unsigned char* buffer = (unsigned char*)malloc(atlas * atlas);
//	if (!buffer)
//		return nullptr;
//	memset(buffer, 0, atlas * atlas);
//	stbtt_PackBegin(&pcontext, buffer, atlas, atlas, 0, 1, NULL);
//	auto PackedChars = new stbtt_packedchar[FontInfo->numGlyphs];
//	stbtt_PackSetSkipMissingCodepoints(&pcontext, true);
//	stbtt_PackFontRange(&pcontext, FontInfo->data, 0, FontHeight, 32, FontInfo->numGlyphs, PackedChars);
//	stbtt_PackEnd(&pcontext);
//	auto rt = Texture::Create(buffer, atlas, atlas, TextureType::Texture2D, TextureFormat::Red, TextureDataFormat::UByte);
//	//rt->Compress();
//	free(buffer);
//	return rt;
//}