#pragma once
#include "Core/Graphics/PipeLine/Material.h"
#include "Core/Graphics/Platform/GLTexture.h"
#pragma region Default Shader
#define DEF_VSHADER \
"#version 330 core\n"\
"layout(location = 0) in vec3 aPos;"\
"out vec4 vertexColor;"\
"void main()"\
"{"\
"	gl_Position = vec4(aPos, 1.0);"\
"	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);"\
" }"
#define DEF_FSHADER \
"#version 330 core\n"\
"out vec4 FragColor;"\
"in vec4 vertexColor;"\
"void main()"\
"{"\
"	FragColor = vertexColor;"\
"}"
#pragma endregion

namespace Sphynx::Core::Graphics::GL{
    class GLMaterial : public Material
    {
    private:
        unsigned int ProgramId = 0;
        static GLMaterial* Bound;
        static GLMaterial DefaultMaterial;
        virtual void i_CreateMaterial(ShaderPack& pack)override;
        static GLMaterial CreateDefaultMaterial();
        void Release();
    public:
        GLMaterial() { ProgramId = 0; };
        inline GLMaterial(ShaderPack& pack, GLTexture* _tex) : Material(pack, _tex) { i_CreateMaterial(pack); };
        virtual void Bind() override;
        GLMaterial(const GLMaterial& mat) = delete;
        GLMaterial& operator=(const GLMaterial& mat) = delete;
        GLMaterial(GLMaterial&& mat)noexcept;
        GLMaterial& operator=(GLMaterial&& mat)noexcept;
        virtual bool IsValid() override { return ProgramId; }
        ~GLMaterial();
        bool IsBound() { return Bound == this; };
        static GLMaterial* GetBoundMaterial() { return Bound; };
        static GLMaterial* GetDefaultMaterial() { return &DefaultMaterial; };
        friend class GLRenderer;
    };
}