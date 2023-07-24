#include "pch.h"
#include "FontAtlas.h"
#include "Core/Graphics/Pipeline/Texture.h"
//#define STB_RECT_PACK_IMPLEMENTATION
//#include "stb_rect_pack.h"
//#define STB_TRUETYPE_IMPLEMENTATION
//#define STBTT_STATIC
//#include "stb_truetype.h"
//#include "stb_image_write.h"

using namespace Sphynx::Core::Graphics;

Sphynx::Core::FontAtlas::FontAtlas()
{

}

void Sphynx::Core::FontAtlas::BuildAtlas()
{
	//Prep Texture Object.
	auto Tex = Texture::Create(TextureType::Texture2D, 0, 0, 0, TextureFormat::Red, TextureDataFormat::UByte);
	for (auto& font : Fonts) {
		auto Fsize = font.GetFontSize();
	}
}
