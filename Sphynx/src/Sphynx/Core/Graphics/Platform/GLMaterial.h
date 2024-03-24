#pragma once
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Platform/GLTexture.h"
#include "Core/Graphics/Platform/GLUniformBuffer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <functional>
#pragma region Default Shader
#define DEF_VSHADER \
"#version 440 core\n"\
"layout(location = 0) in vec3 aPos;"\
"layout(location = 1) in vec2 texcoord;"\
"uniform mat4 MVP;"\
"out vec4 vertexColor;"\
"out vec2 TexCoord;"\
"void main()"\
"{"\
"	gl_Position = MVP * vec4(aPos, 1.0);"\
"	vertexColor = vec4(1.0, 1.0, 1.0, 1.0);"\
"   TexCoord = texcoord;"\
" }"
#define DEF_FSHADER \
"#version 440 core\n"\
"out vec4 FragColor;"\
"in vec4 vertexColor;"\
"in vec2 TexCoord;"\
"void main()"\
"{"\
"   FragColor = vertexColor;"\
"}"
#pragma endregion

namespace Sphynx::Core::Graphics::GL {
    class GLUniform final : public Uniform {
        void* Data;
        int loc = -1;
        ShaderDataType Type;
    public:
        void BindLocation() { Set(loc); };
        GLUniform(void* data, ShaderDataType type);
        virtual void SetData(void* data) { Data = data; };
        virtual void Set(const int loc);
        friend class GLMaterial;
        friend class GLRenderer;
    };
    class GLMaterial final : public Material
    {
    private:
        std::atomic_bool ShouldValidate;
        unsigned int ProgramId = -1;
        std::list<Texture*> textures;
        std::list<Uniform*> uniforms;
        ShaderPack Shaders = { 0, 0, 0, 0, 0 };
        static GLMaterial* Bound;
        static GLMaterial DefaultMaterial;
        //Gets Called By the Renderer To Ensure that this gets called after OpenGL Context is created.
        static GLMaterial CreateDefaultMaterial();
        void Release();
        void SharedInit(const ShaderPack& shaders);
        //Texture Missing.
    public:
        GLMaterial() noexcept { ProgramId = 0; };
        GLMaterial(const ShaderPack& shaders);
        GLMaterial(const ShaderPack& pack, Texture* _tex);
        GLMaterial(const ShaderPack& shaders, std::initializer_list<Texture*> _tex);
        ~GLMaterial();
        //Binds The Program.
        virtual void Bind()noexcept override;
        virtual void Unbind()noexcept override;
        virtual bool IsValid()const noexcept override { return ProgramId != -1; }
        virtual void SetUniform(Uniform* uniform, const char* name);
        virtual void SetUniform(Uniform* uniform, const int index);
        virtual void SetUniformBuffer(UniformBuffer* Ubuf, const char* name);
        virtual void SetUniformBuffer(UniformBuffer* Ubuf, const int Index);
        virtual void AddTexture(Texture* texture);
        virtual void RemoveTexture(Texture* texture) { textures.remove(texture); };
        virtual void RemoveTexture(unsigned int index) {
            int i = 0;
            for (auto& tex : textures) {
                if (i == index) { textures.remove(tex); break; }
                i++;
            }
        }
        virtual void ClearTextures() { textures.clear(); };
        virtual void SetTexture(Texture* texture, unsigned int index) {
            int i = 0;
            for (auto& tex : textures) {
                if (i == index) {
                    (*tex) = *texture;
                }
            }
        }
        virtual Texture* GetTexture(unsigned int index)noexcept
        {
            if (index >= textures.size())return nullptr;
            int i = 0;
            for (auto& tex : textures) {
                if (i == index)return tex;
                i++;
            }
        };
        virtual unsigned int GetTextureCount()const noexcept { return textures.size(); };
        //Returns the location of the uniform with the specified name, or -1 in case of an error.
        //Will not throw an exception.
        //implementation details : this function will attempt to read the return value from the rendering thread and will timeout after 100ms if failed and return -1.
        virtual const int GetUniformLocation(const char* name) noexcept;
        virtual void ReloadShaders(const ShaderPack& pack);
        virtual void ReloadShaders(Shader* shader);
        virtual const ShaderPack& GetShaders()const { return Shaders; };
        GLMaterial(const GLMaterial& mat) = delete;
        GLMaterial& operator=(const GLMaterial& mat) = delete;
        GLMaterial(GLMaterial&& mat)noexcept;
        GLMaterial& operator=(GLMaterial&& mat)noexcept;
        //Checks Whether the Material Is Valid i.e. Succesfully Linked and created.
        bool IsBound()const noexcept { return Bound == this; };
        //Getters
        static GLMaterial* GetBoundMaterial() noexcept { return Bound; };
        //Returns the Default Material.
        static GLMaterial* GetDefaultMaterial()noexcept { return &DefaultMaterial; };
        static GLMaterial* CreateDefaultMaterialCopy() noexcept {
            return (GLMaterial*)Create(DefaultMaterial.Shaders);
        };
        static Shader* GetDefaultShader(ShaderType type);
        int GetAttributeLocation(std::string name);
        friend class GLRenderer;
    };
}