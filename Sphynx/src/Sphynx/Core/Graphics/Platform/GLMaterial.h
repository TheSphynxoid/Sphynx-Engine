#pragma once
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Platform/GLTexture.h"
#include "Core/Graphics/Platform/GLUniformBuffer.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <functional>
#pragma region Default Shader
#define DEF_VSHADER \
"#version 330 core\n"\
"layout(location = 0) in vec3 aPos;"\
"layout(location = 1) in vec2 texcoord;"\
"uniform mat4 MVP;"\
"out vec4 vertexColor;"\
"out vec2 TexCoord;"\
"void main()"\
"{"\
"	gl_Position = MVP * vec4(aPos, 1.0);"\
"	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);"\
"   TexCoord = texcoord;"\
" }"
#define DEF_FSHADER \
"#version 330 core\n"\
"out vec4 FragColor;"\
"in vec4 vertexColor;"\
"in vec2 TexCoord;"\
"void main()"\
"{"\
"	FragColor = vertexColor;"\
"}"
#pragma endregion

namespace Sphynx::Core::Graphics::GL {
    class GLUniform final : public Uniform {
        ShaderDataType Type;
        void* Data;
        int loc = -1;
    public:
        void Set() { Set(loc); };
        GLUniform(void* data, ShaderDataType type);
        virtual void SetData(void* data) { Data = data; };
        virtual void Set(const int loc);
        friend class GLMaterial;
        friend class GLRenderer;
    };
    class GLMaterial final : public Material
    {
    private:
        unsigned int ProgramId = 0;
<<<<<<< Updated upstream
=======
        std::list<Texture*> textures;
        std::list<Uniform*> uniforms;
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        static GLMaterial* Bound;
        static GLMaterial DefaultMaterial;
        //Creates The GL Program.
        virtual void i_CreateMaterial(const ShaderPack& pack)override;
        //Gets Called By the Renderer To Ensure that this gets called after OpenGL Context is created.
        static GLMaterial CreateDefaultMaterial();
        void Release();
        //Texture Missing.
    public:
        GLMaterial() { ProgramId = 0; };
<<<<<<< Updated upstream
        GLMaterial(ShaderPack&& pack, GLTexture* _tex) : Material(pack, _tex) { i_CreateMaterial(pack); };
        //Binds The Program.
        virtual void Bind() override;
=======
        GLMaterial(const ShaderPack& shaders);
        GLMaterial(const ShaderPack& pack, Texture* _tex);
        GLMaterial(const ShaderPack& shaders, std::initializer_list<Texture*> _tex);
        ~GLMaterial();
        //Binds The Program.
        virtual void Bind() override;
        virtual void Unbind() override;
        virtual bool IsValid() override { return ProgramId; }
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
        virtual void RemoveTexture() { textures.clear(); };
        virtual void SetTexture(Texture* texture, unsigned int index) {
            int i = 0;
            for (auto& tex : textures) {
                if (i == index) {
                    (*tex) = *texture;
                }
            }
        }
        virtual Texture* GetTexture(unsigned int index) 
        {
            int i = 0;
            for (auto& tex : textures) {
                if (i == index)return tex;
                i++;
            }
        };
        virtual unsigned int GetTextureCount() { return textures.size(); };
        virtual const unsigned int GetUniformLocation(const char* name);
        virtual void ReloadShaders(const ShaderPack& pack);
        virtual Shader* GetDefaultShader(ShaderType type);
<<<<<<< Updated upstream
<<<<<<< Updated upstream
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
        GLMaterial(const GLMaterial& mat) = delete;
        GLMaterial& operator=(const GLMaterial& mat) = delete;
        GLMaterial(GLMaterial&& mat)noexcept;
        GLMaterial& operator=(GLMaterial&& mat)noexcept;
        //Checks Whether the Material Is Valid i.e. Succesfully Linked and created.
        bool IsBound() { return Bound == this; };
        //Getters
        static GLMaterial* GetBoundMaterial() { return Bound; };
        //Returns the Default Material.
        static GLMaterial* GetDefaultMaterial() { return &DefaultMaterial; };
        static GLMaterial* CreateDefaultMaterialCopy();
        int GetAttributeLocation(std::string name);
        friend class GLRenderer;
    };
}