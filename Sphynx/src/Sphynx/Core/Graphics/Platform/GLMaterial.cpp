#include "pch.h"
#include "GLMaterial.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "glad/glad.h"

using namespace Sphynx::Core::Graphics::GL;

GLMaterial* GLMaterial::Bound = nullptr;
GLMaterial GLMaterial::DefaultMaterial;

void GLMaterial::i_CreateMaterial(ShaderPack& shaders){
    ProgramId = glCreateProgram();
    glUseProgram(ProgramId);
    glAttachShader(ProgramId,((GLShader*)shaders.Vert)->id);
    glAttachShader(ProgramId,((GLShader*)shaders.Frag)->id);
    if(shaders.Geom != NULL){
        glAttachShader(ProgramId,((GLShader*)shaders.Geom)->id);
    }
    if(shaders.Tess != NULL){
        glAttachShader(ProgramId,((GLShader*)shaders.Tess)->id);
    }
    glLinkProgram(ProgramId);
    int success;
    glGetProgramiv(ProgramId, GL_LINK_STATUS, &success);
    if (!success) {
        char log[1024];
        glGetProgramInfoLog(ProgramId, 512, NULL, log);
        Core_Error(log);
        return;
    }
}

GLMaterial Sphynx::Core::Graphics::GL::GLMaterial::CreateDefaultMaterial()
{
    GLMaterial mat = GLMaterial();
    GLShader vert = GLShader();
    GLShader frag = GLShader();
    vert.CreateFromCode(DEF_VSHADER, ShaderType::VertexShader);
    frag.CreateFromCode(DEF_FSHADER, ShaderType::FragmentShader);
    ShaderPack pack = ShaderPack(&vert, &frag, nullptr, nullptr);
    mat.CreateMaterial(pack, nullptr);
    return mat;
}

void Sphynx::Core::Graphics::GL::GLMaterial::Release()
{
    glDeleteProgram(ProgramId);
    ProgramId = 0;
}

void GLMaterial::Bind()
{
    glUseProgram(ProgramId);
    Bound = this;
}

Sphynx::Core::Graphics::GL::GLMaterial::GLMaterial(GLMaterial&& mat) noexcept
{
    std::swap(ProgramId, mat.ProgramId);
    std::swap(Textures, mat.Textures);
}

GLMaterial& Sphynx::Core::Graphics::GL::GLMaterial::operator=(GLMaterial&& mat) noexcept
{
    if (this != &mat) {
        Release();
        std::swap(ProgramId, mat.ProgramId);
        std::swap(Textures, mat.Textures);
    }
    return *this;
}

Sphynx::Core::Graphics::GL::GLMaterial::~GLMaterial()
{
    if (IsBound())Bound = nullptr;
    Release();
}
