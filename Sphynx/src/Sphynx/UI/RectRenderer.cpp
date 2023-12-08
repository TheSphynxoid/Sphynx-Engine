#include "pch.h"
#include "RectRenderer.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Application.h"
#include "Core/Graphics/Window.h"

using namespace Sphynx::Core::Graphics;
using namespace Sphynx::Core;

void Sphynx::UI::RectRenderer::OnComponentAttach(GameObject* parent)
{
}

void Sphynx::UI::RectRenderer::OnComponentDetach()
{
}

void Sphynx::UI::RectRenderer::Update()
{
	GetMainWindow()->GetRenderer()->Submit(RO);
}

Sphynx::UI::RectRenderer::RectRenderer(glm::vec2 bounds, Core::Graphics::Texture* texture)
{
	float vertices[] = {
		// positions        // texture coords
		1.0f,  1.0f, 0.0f, 1.0f, 1.0f,   // top right
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom right
	   -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,   // bottom left
	   -1.0f,  1.0f, 0.0f, 0.0f, 1.0f    // top left 
	};
	unsigned int index[] = {
		0, 1, 2,
		2, 3, 0
	};
	auto vb = Sphynx::Core::Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
	auto ib = Sphynx::Core::Graphics::IndexBuffer::Create(index, sizeof(index));
	vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3,false),BufferElement(ShaderDataType::Float2,false) }));
	rectMesh = Core::Graphics::Mesh::Create(vb, ib);
#if defined(Platform_Windows) && defined(DX_IMPL)
#else
	auto vs = Core::Graphics::Shader::Create("#version 440 core\n"
		"layout(location = 0) in vec2 aPos;\n"
		"layout(location = 1) in vec2 aTexCoords;\n"
		"out vec2 TexCoords;\n"
		"void main(){gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);TexCoords = aTexCoords;}", Core::Graphics::ShaderType::VertexShader);
	auto fs = Core::Graphics::Shader::Create("#version 440 core\n"
		"out vec4 FragColor;\n"
		"in vec2 TexCoords;\n"
		"uniform sampler2D screenTexture;\n"
		"void main() { FragColor = texture(screenTexture, TexCoords) * vec4(1,1,1,1); }", Core::Graphics::ShaderType::FragmentShader);
#endif
	rectMaterial = Core::Graphics::Material::Create(Core::Graphics::ShaderPack(vs, fs, 0, 0, 0), texture);
	RO = Core::Graphics::RenderObject(rectMesh, rectMaterial);
}
