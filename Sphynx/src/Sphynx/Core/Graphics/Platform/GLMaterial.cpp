#include "pch.h"
#include "GLMaterial.h"
#include "GLShader.h"
#include "GLTexture.h"
//#include "glad/glad.h"

using namespace Sphynx::Core::Graphics::GL;

GLMaterial* GLMaterial::Bound = nullptr;
GLMaterial GLMaterial::DefaultMaterial;

void Sphynx::Core::Graphics::GL::UniformData::SetUniform(Sphynx::Core::Graphics::GL::GLMaterial* Mat)
{
    Mat->SetUniformValue(*this);
}

void GLMaterial::i_CreateMaterial(const ShaderPack& shaders){
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
    GLShader::DefaultVertexShader = new GLShader();
    GLShader::DefaultFragmentShader = new GLShader();
    GLShader::DefaultVertexShader->CreateFromCode(DEF_VSHADER, ShaderType::VertexShader);
    GLShader::DefaultFragmentShader->CreateFromCode(DEF_FSHADER, ShaderType::FragmentShader);
    ShaderPack pack = ShaderPack(GLShader::DefaultVertexShader, GLShader::DefaultFragmentShader, nullptr, nullptr);
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

int Sphynx::Core::Graphics::GL::GLMaterial::GetAttributeLocation(std::string name)
{
    return glGetAttribLocation(ProgramId, name.c_str());
}

int Sphynx::Core::Graphics::GL::GLMaterial::GetUniformLocation(std::string name)
{
    return glGetUniformLocation(ProgramId,name.c_str());
}

void Sphynx::Core::Graphics::GL::GLMaterial::SetUniformValue(Sphynx::Core::Graphics::GL::UniformData data)
{
    //Hopefully this works.
    data.command();
}