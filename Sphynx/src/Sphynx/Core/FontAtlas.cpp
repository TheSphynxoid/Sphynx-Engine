#include "pch.h"
#include "FontAtlas.h"
#include "Core/Graphics/Pipeline/Texture.h"
#define STB_RECT_PACK_IMPLEMENTATION
#include "stb_rect_pack.h"
#define STB_TRUETYPE_IMPLEMENTATION
#define STBTT_STATIC
#include "stb_truetype.h"
#include "stb_image_write.h"

Sphynx::Core::Graphics::Texture* Sphynx::Core::FontAtlas::AllocateTexture()
{
	return nullptr;
}

Sphynx::Core::FontAtlas::FontAtlas()
{

}

void Sphynx::Core::FontAtlas::PrepareAtlas()
{
	auto Tex = AllocateTexture();
	DataBuffer TBMap = { Tex->GetTextureBuffer()->Map(Graphics::MapAccess::Write) , 
		Tex->GetWidth() * Tex->GetHeight() * Tex->GetBitsPerPixel() };
	auto Ranges = Font::GetLatinRange();
}
