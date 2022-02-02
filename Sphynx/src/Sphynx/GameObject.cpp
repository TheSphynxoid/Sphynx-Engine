#include "pch.h"
#include "GameObject.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/MeshRenderer.h"

using namespace Sphynx::Core::Graphics;

const Sphynx::GameObject Sphynx::GameObject::PlaceHolder = Sphynx::GameObject("PlaceHolder");

Sphynx::GameObject::GameObject(char* name) : IsAlive(true), InstanceID((size_t)this), Name(name)
{
    //((Component*)&transform)->Parent = this;
    //((Component*)&transform)->_transform = transform;
}

Sphynx::GameObject::GameObject(const GameObject& obj)noexcept
{
    this->Components = obj.Components;
    this->transform = obj.transform;
    this->IsAlive = obj.IsAlive;
    //InstanceID = (size_t)this;
    this->InstanceID = obj.InstanceID;
    this->Name = obj.Name;
    //Core::Internal::ComponentFactory::CompCopy(const_cast<GameObject*>(&obj), this);
}
//Move Constructor.
Sphynx::GameObject::GameObject(GameObject&& obj)noexcept
{
    std::swap(this->Components, obj.Components);
    std::swap(this->transform, obj.transform);
    std::swap(this->IsAlive, obj.IsAlive);
    std::swap(this->Name, obj.Name);
    this->InstanceID = obj.InstanceID;
}

Sphynx::GameObject::~GameObject()
{
    for (auto& comp : Components) {
        comp->OnComponentDetach();
        Components.remove(comp);
    }
}

Sphynx::GameObject& Sphynx::GameObject::operator=(GameObject&& obj)noexcept
{
    if (this != &obj) {
        std::swap(this->Components, obj.Components);
        std::swap(this->IsAlive, obj.IsAlive);
        std::swap(this->transform, obj.transform);
        std::swap(this->Name, obj.Name);
    }
    return *this;
}

Sphynx::GameObject& Sphynx::GameObject::operator=(const GameObject& obj)noexcept
{
    this->Components = obj.Components;
    this->IsAlive = obj.IsAlive;
    this->transform = obj.transform;
    this->InstanceID = obj.InstanceID;
    return *this;
}

Sphynx::GameObject Sphynx::GameObject::CreatePrimitive(Primitives primitive, char* name)
{
    auto rt = GameObject(name);
    VertexBuffer* vb = nullptr;
    IndexBuffer* ib = nullptr;
    switch (primitive)
    {
    case Sphynx::Primitives::Sphere:
        Core_Debug_Message("Sphere Rendering Not Implemented");
        break;
    case Sphynx::Primitives::Capsule:
        Core_Debug_Message("Capsule Rendering Not Implemented");
        break;
    case Sphynx::Primitives::Cube:
    {
        float vertices[] = {
             // positions       // texture coords
            -1, -1, -1, 1, 1,   // top right
             1, -1, -1, 1, 0,   // bottom right
             1,  1, -1, 0, 0,   // bottom left
            -1,  1, -1, 0, 1,   // top left
            -1, -1,  1, 1, 1,
             1, -1,  1, 1, 0,
             1,  1,  1, 0, 0,
            -1,  1,  1, 0, 1
        };
        unsigned int indices[] =
        {
            0, 1, 3, 3, 1, 2,
            1, 5, 2, 2, 5, 6,
            5, 4, 6, 6, 4, 7,
            4, 0, 7, 7, 0, 3,
            3, 2, 7, 7, 2, 6,
            4, 5, 0, 0, 5, 1
        };
        vb = VertexBuffer::Create(vertices, sizeof(vertices));
        ib = IndexBuffer::Create(indices, sizeof(indices));
        vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3,false),BufferElement(ShaderDataType::Float2,false) }));
        break;
    }
    case Sphynx::Primitives::Plane:
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
        vb = VertexBuffer::Create(vertices, sizeof(vertices));
        ib = IndexBuffer::Create(index, sizeof(index));
        vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3,false),BufferElement(ShaderDataType::Float2,false) }));
        break;
    }
    case Sphynx::Primitives::Triangle:
    {
        float vertices[] = {
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             0.0f,  1.0f, 0.0f, 0.5f, 1.0f
        };
        vb = VertexBuffer::Create(vertices, sizeof(vertices));
        vb->SetDataLayout(BufferLayout({ BufferElement(ShaderDataType::Float3,false),BufferElement(ShaderDataType::Float2,false) }));
        break;
    }
    default:
        break;
    }
    auto Mesh = Mesh::Create(vb, ib);
    rt.AddComponent<MeshRenderer>(Mesh, Material::GetDefaultMaterialCopy());
    return rt;
}
