#include "pch.h"
#include "TextRenderer.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Pipeline/Mesh.h"


using namespace Sphynx::Core;

void Sphynx::Core::TextRenderer::OnComponentAttach(GameObject* parent)
{

}

void Sphynx::Core::TextRenderer::RenderLoop()
{
	for (auto b = String.begin(); b < String.end(); b++) {
		auto r = font->GetCharacterBitmap(*b);
	}
}

Sphynx::Core::TextRenderer::TextRenderer(Font* nfont, std::string st) : font(nfont), String(st)
{
	Atlas = FontAtlas();
	Atlas.AddFont(nfont);
	Atlas.PrepareAtlas();
}

Sphynx::Core::TextRenderer::~TextRenderer()
{
}

void Sphynx::Core::TextRenderer::Update()
{
}
