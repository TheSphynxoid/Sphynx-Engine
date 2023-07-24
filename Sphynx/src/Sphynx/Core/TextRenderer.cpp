#include "pch.h"
#include "TextRenderer.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Pipeline/Mesh.h"


using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

void Sphynx::Core::TextRenderer::OnComponentAttach(GameObject* parent)
{

}

void Sphynx::Core::TextRenderer::RenderLoop()
{
	for (auto b = String.begin(); b < String.end(); b++) {
		auto r = font->GetCharacterBitmap(*b);
	}
}

void Sphynx::Core::TextRenderer::GenQuads()
{
	Quads = std::vector<Quad>(String.size());
	for (auto& c : String) {
		Quads.push_back(font->GetCharacterQuad(c));
	}
}

Sphynx::Core::TextRenderer::TextRenderer(Font* nfont, std::string st) : font(nfont), String(st)
{
	Atlas = FontAtlas();
	Atlas.AddFont(nfont);
	GenQuads();
}

Sphynx::Core::TextRenderer::~TextRenderer()
{
}

void Sphynx::Core::TextRenderer::Update()
{
}
