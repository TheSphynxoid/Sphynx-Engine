#include "pch.h"
#include "SpriteRenderer.h"
#include "Core/Graphics/Pipeline/Shader.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "GameObject.h"
#include "SceneManager.h"

using namespace Sphynx::Core::Graphics;

auto OrthoProj = glm::mat4(0.0f);
auto ProjView = OrthoProj;
Uniform* ProjViewUniform = Uniform::Create(&ProjView[0], ShaderDataType::Mat4x4);
bool HasInit = false;
//Box
float vertices[] = {
    // pos      // tex
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
};
auto vShader = Shader::Create("#version 330 core\n"
    "layout(location = 0) in vec4 vertex;"
    "out vec2 TexCoords;"
    "uniform mat4 model;"
    "uniform mat4 projection;"
    "void main()"
    "{"
    "TexCoords = vertex.zw;"
    "gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);"
    "}", ShaderType::VertexShader);
auto fShader = Shader::Create("#version 330 core\n"
    "in vec2 TexCoords;"
    "out vec4 color;"
    "uniform sampler2D image;"
    "void main()"
    "{"
    "color = texture(Sprite, TexCoord) * vec4(1,1,1,1);"
    "}", ShaderType::FragmentShader);

void FrameResize(Sphynx::OnFrameResize& e)
{
    OrthoProj = glm::ortho(0.0f, e.NewDimensions.x, 0.0f, e.NewDimensions.y, -1.0f, 1.0f);
}

Sphynx::Core::SpriteRenderer::SpriteRenderer()
{
    if (!HasInit) {
        HasInit = true;
        OrthoProj = glm::ortho(0.0f, (float)GetMainWindow()->GetWidth(), 0.0f, (float)GetMainWindow()->GetHeight(), 0.1f, 1.0f);
    }
    SpriteMaterial = Material::Create({ vShader,fShader,nullptr,nullptr,nullptr });
    //Orthographic projection matrix.
    auto vb = Sphynx::Core::Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
    vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float4,false) }));
    SpritePlane = Graphics::Mesh::Create(vb, nullptr);

    Sphynx::Events::GlobalEventSystem::GetInstance()->Subscribe<Sphynx::OnFrameResize>(&FrameResize);
}

Sphynx::Core::SpriteRenderer::SpriteRenderer(Sphynx::Core::Graphics::Material* mat, glm::vec2 size)
{
    if (!HasInit) {
        HasInit = true;
        OrthoProj = glm::ortho(0.0f, (float)GetMainWindow()->GetWidth(), 0.0f, (float)GetMainWindow()->GetHeight(), 0.1f, 1.0f);
    }

    //Orthographic projection matrix.
    auto vb = Sphynx::Core::Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
    vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float4,false) }));
    SpritePlane = Graphics::Mesh::Create(vb, nullptr);
    SpriteMaterial = mat;
    Size = size;

    Sphynx::Events::GlobalEventSystem::GetInstance()->Subscribe<Sphynx::OnFrameResize>(&FrameResize);
}

Sphynx::Core::SpriteRenderer::SpriteRenderer(Sphynx::Core::Graphics::Texture* tex, glm::vec2 size)
{
    SpriteMaterial = Material::Create({ vShader,fShader,nullptr,nullptr,nullptr }, tex);
    //Orthographic projection matrix.
    auto vb = Sphynx::Core::Graphics::VertexBuffer::Create(vertices, sizeof(vertices));
    vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float4,false) }));
    SpritePlane = Graphics::Mesh::Create(vb, nullptr);
    Size = size;

    Sphynx::Events::GlobalEventSystem::GetInstance()->Subscribe<Sphynx::OnFrameResize>(FrameResize);
}

void Sphynx::Core::SpriteRenderer::SetCenter(glm::vec2 Center)
{
    CenterOffset = -Center;
    this->GetTransform()->Translate(GetTransform()->GetPosition() + glm::vec3(CenterOffset, 0));
}

void Sphynx::Core::SpriteRenderer::OnComponentAttach(GameObject* parent)
{
    if (!HasInit) {
        auto cam = SceneManager::GetScene().GetPrimaryCamera();
        auto camv = cam->GetViewport();
        HasInit = true;
        OrthoProj = glm::ortho(0.0f, (float)camv.Width, 0.0f, (float)camv.Width, 0.1f, 1.0f);
        ProjView = OrthoProj * cam->GetViewMatrix();
    }

    parent->GetTransform()->SetOrigin({ Size.x / 2, Size.y / 2,0 });
    parent->GetTransform()->Scale(glm::vec3(Size, 0));

    ModelUniform = Graphics::Uniform::Create(&parent->GetTransform()->GetModelMatrix()[0], ShaderDataType::Mat4x4);
    SpriteMaterial->SetUniform(ModelUniform, 1);
    SpriteMaterial->SetUniform(ProjViewUniform, 2);
}

void Sphynx::Core::SpriteRenderer::Update()
{
    if (GetTransform()->Changed) {
        ModelUniform->SetData(&GetGameObject()->GetTransform()->GetModelMatrix()[0]);
    }
    if (SceneManager::GetScene().GetPrimaryCamera()->GetTransform()->Changed) {
        auto cam = SceneManager::GetScene().GetPrimaryCamera();
        ProjView = OrthoProj * cam->GetViewMatrix();
        ProjViewUniform->SetData(&ProjView[0]);
    }
    GetMainWindow()->GetRenderer()->Submit({ SpritePlane,SpriteMaterial });
}
