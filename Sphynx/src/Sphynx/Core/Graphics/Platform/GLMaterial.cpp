#include "pch.h"
#include "GLMaterial.h"
#include "GLShader.h"
#include "GLTexture.h"
//#include "glad/glad.h"

using namespace Sphynx::Core::Graphics::GL;

GLMaterial* GLMaterial::Bound = nullptr;
GLMaterial GLMaterial::DefaultMaterial;

Sphynx::Core::Graphics::GL::GLMaterial::GLMaterial(const ShaderPack& shaders) : Shaders(shaders)
{
    ProgramId = glCreateProgram();
    glUseProgram(ProgramId);
    glAttachShader(ProgramId, ((GLShader*)shaders.Vert)->id);
    glAttachShader(ProgramId, ((GLShader*)shaders.Frag)->id);
    if (shaders.Geom != NULL) {
        glAttachShader(ProgramId, ((GLShader*)shaders.Geom)->id);
    }
    if (shaders.TessEval != NULL) {
        glAttachShader(ProgramId, ((GLShader*)shaders.TessEval)->id);
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

Sphynx::Core::Graphics::GL::GLMaterial::GLMaterial(const Sphynx::Core::Graphics::ShaderPack& shaders, Sphynx::Core::Graphics::Texture* _tex)
    : textures(), Shaders(shaders)
{
    textures.push_back(_tex);
    ProgramId = glCreateProgram();
    glAttachShader(ProgramId,((GLShader*)shaders.Vert)->id);
    glAttachShader(ProgramId,((GLShader*)shaders.Frag)->id);
    if(shaders.Geom != NULL){
        glAttachShader(ProgramId,((GLShader*)shaders.Geom)->id);
    }
    if (shaders.TessEval != NULL) {
        glAttachShader(ProgramId, ((GLShader*)shaders.TessEval)->id);
    }
    //Handling if Tess is enabled and TessEval is null.
    if (shaders.TessControl != NULL && shaders.TessEval == nullptr) {
        Core_Error("Ignoring Tessellation Because Tessellation Evaluation Shader is null");
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

Sphynx::Core::Graphics::GL::GLMaterial::GLMaterial(const ShaderPack& shaders, std::initializer_list<Texture*> _tex) : textures(_tex), Shaders(shaders)
{
    
    ProgramId = glCreateProgram();
    glUseProgram(ProgramId);
    glAttachShader(ProgramId, ((GLShader*)shaders.Vert)->id);
    glAttachShader(ProgramId, ((GLShader*)shaders.Frag)->id);
    if (shaders.Geom != NULL) {
        glAttachShader(ProgramId, ((GLShader*)shaders.Geom)->id);
    }
    if (shaders.TessEval != NULL) {
        glAttachShader(ProgramId, ((GLShader*)shaders.TessEval)->id);
    }
    //Handling if Tess is enabled and TessEval is null.
    if (shaders.TessControl != NULL && shaders.TessEval == nullptr) {
        Core_Error("Ignoring Tessellation Because Tessellation Evaluation Shader is null");
    }
    else {
        glAttachShader(ProgramId, ((GLShader*)shaders.TessControl)->id);
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

void Sphynx::Core::Graphics::GL::GLMaterial::SetUniform(Uniform* uniform, const char* name)
{
    uniforms.push_back(uniform);
    Bind();
    auto loc = GetUniformLocation(name);
    ((GLUniform*)uniform)->Set(loc);
    Unbind();
}

void Sphynx::Core::Graphics::GL::GLMaterial::SetUniform(Uniform* uniform, const int index)
{
    uniforms.push_back(uniform);
    Bind();
    ((GLUniform*)uniform)->Set(index);
    Unbind();
}

void Sphynx::Core::Graphics::GL::GLMaterial::SetUniformBuffer(UniformBuffer* Ubuf, const char* name)
{
    throw std::bad_exception();
}

void Sphynx::Core::Graphics::GL::GLMaterial::SetUniformBuffer(UniformBuffer* Ubuf, const int Index)
{
    throw std::bad_exception();
}

void Sphynx::Core::Graphics::GL::GLMaterial::AddTexture(Texture* texture)
{
    this->textures.push_back(texture);
}

const unsigned int Sphynx::Core::Graphics::GL::GLMaterial::GetUniformLocation(const char* name)
{
    return glGetUniformLocation(ProgramId, name);
}

void Sphynx::Core::Graphics::GL::GLMaterial::ReloadShaders(const ShaderPack& pack)
{
    glDeleteProgram(ProgramId);
    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, ((GLShader*)pack.Vert)->id);
    glAttachShader(ProgramId, ((GLShader*)pack.Frag)->id);
    if (pack.Geom != NULL) {
        glAttachShader(ProgramId, ((GLShader*)pack.Geom)->id);
    }
    if (pack.TessEval != NULL) {
        glAttachShader(ProgramId, ((GLShader*)pack.TessEval)->id);
    }
    //Handling if Tess is enabled and TessEval is null.
    if (pack.TessControl != NULL && pack.TessEval == nullptr) {
        Core_Error("Ignoring Tessellation Because Tessellation Evaluation Shader is null");
    }
    else {
        glAttachShader(ProgramId, ((GLShader*)pack.TessControl)->id);
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
    Bind();
    for (auto& uni : uniforms) {
        ((GLUniform*)uni)->BindLocation();
    }
    Unbind();
}

void Sphynx::Core::Graphics::GL::GLMaterial::ReloadShaders(Shader* shader)
{
    Shaders.ReplaceShader(shader);
    ReloadShaders(Shaders);
}

Sphynx::Core::Graphics::Shader* Sphynx::Core::Graphics::GL::GLMaterial::GetDefaultShader(ShaderType type)
{
    switch (type)
    {
    case Sphynx::Core::Graphics::ShaderType::VertexShader:
        return GLShader::DefaultVertexShader;
    case Sphynx::Core::Graphics::ShaderType::FragmentShader:
        return GLShader::DefaultFragmentShader;
    default:
        return nullptr;
    }
}

GLMaterial Sphynx::Core::Graphics::GL::GLMaterial::CreateDefaultMaterial()
{
    GLShader::DefaultVertexShader = new GLShader(DEF_VSHADER, ShaderType::VertexShader);
    GLShader::DefaultFragmentShader = new GLShader(DEF_FSHADER, ShaderType::FragmentShader);
    ShaderPack pack = ShaderPack(GLShader::DefaultVertexShader, GLShader::DefaultFragmentShader, nullptr, nullptr, nullptr);
    GLMaterial mat = GLMaterial(pack);
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
    int i = 0;
    for (auto& tex : textures) {
        glActiveTexture(GL_TEXTURE0 + i);
        tex->Bind();
        ++i;
    }
    Bound = this;
}

void Sphynx::Core::Graphics::GL::GLMaterial::Unbind()
{
    for (auto& tex : textures) {
        tex->Unbind();
    }
    glUseProgram(0);
    Bound = nullptr;
}

Sphynx::Core::Graphics::GL::GLMaterial::GLMaterial(GLMaterial&& mat) noexcept
{
    std::swap(ProgramId, mat.ProgramId);
    std::swap(textures, mat.textures);
}

GLMaterial& Sphynx::Core::Graphics::GL::GLMaterial::operator=(GLMaterial&& mat) noexcept
{
    if (this != &mat) {
        Release();
        std::swap(ProgramId, mat.ProgramId);
        std::swap(textures, mat.textures);
    }
    return *this;
}

Sphynx::Core::Graphics::GL::GLMaterial::~GLMaterial()
{
    if (IsBound())Bound = nullptr;
    Release();
}

GLMaterial* Sphynx::Core::Graphics::GL::GLMaterial::CreateDefaultMaterialCopy()
{
    auto m = (GLMaterial*)Create({ GLShader::DefaultVertexShader, GLShader::DefaultFragmentShader, nullptr, nullptr, nullptr });
    return m;
}

int Sphynx::Core::Graphics::GL::GLMaterial::GetAttributeLocation(std::string name)
{
    return glGetAttribLocation(ProgramId, name.c_str());
}

void Sphynx::Core::Graphics::GL::GLUniform::Set(const int _loc)
{
    this->loc = _loc;
    switch (Type)
    {
    case Sphynx::Core::Graphics::ShaderDataType::Float:
        glUniform1f(loc, *(GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Float2:
        //this is awkward. this is propably code smell, i think this will be problematic.
        glUniform2f(loc, ((GLfloat*)Data)[0], ((GLfloat*)Data)[1]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Float3:
        glUniform3f(loc, ((GLfloat*)Data)[0], ((GLfloat*)Data)[1], ((GLfloat*)Data)[2]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Float4:
        glUniform4f(loc, ((GLfloat*)Data)[0], ((GLfloat*)Data)[1], ((GLfloat*)Data)[2], ((GLfloat*)Data)[3]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Double:
        glUniform1d(loc, ((GLdouble*)Data)[0]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Double2:
        glUniform2d(loc, ((GLdouble*)Data)[0], ((GLdouble*)Data)[1]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Double3:
        glUniform3d(loc, ((GLdouble*)Data)[0], ((GLdouble*)Data)[1], ((GLdouble*)Data)[2]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Double4:
        glUniform4d(loc, ((GLdouble*)Data)[0], ((GLdouble*)Data)[1], ((GLdouble*)Data)[2], ((GLdouble*)Data)[3]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Int:
        glUniform1d(loc, ((GLint*)Data)[0]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Int2:
        glUniform2d(loc, ((GLint*)Data)[0], ((GLint*)Data)[1]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Int3:
        glUniform3d(loc, ((GLint*)Data)[0], ((GLint*)Data)[1], ((GLint*)Data)[2]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Int4:
        glUniform4d(loc, ((GLint*)Data)[0], ((GLint*)Data)[1], ((GLint*)Data)[2], ((GLint*)Data)[3]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::UInt:
        glUniform1d(loc, ((GLuint*)Data)[0]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::UInt2:
        glUniform2d(loc, ((GLuint*)Data)[0], ((GLuint*)Data)[1]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::UInt3:
        glUniform3d(loc, ((GLuint*)Data)[0], ((GLuint*)Data)[1], ((GLuint*)Data)[2]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::UInt4:
        glUniform4d(loc, ((GLuint*)Data)[0], ((GLuint*)Data)[1], ((GLuint*)Data)[2], ((GLuint*)Data)[3]);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Bool:
        Core_Error("EHHHH, search for bool uniform");
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat2x2:
        glUniformMatrix2fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat2x3:
        glUniformMatrix2x3fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat2x4:
        glUniformMatrix2x4fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat3x2:
        glUniformMatrix3x2fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat3x3:
        glUniformMatrix3fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat3x4:
        glUniformMatrix3x4fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat4x2:
        glUniformMatrix4x2fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat4x3:
        glUniformMatrix4x3fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::Mat4x4:
        glUniformMatrix4fv(loc, 1, GL_FALSE, (GLfloat*)Data);
        break;
    case Sphynx::Core::Graphics::ShaderDataType::None:
        __fallthrough;
    default:
        Core_Error("Invalid Uniform Data Type");
        break;
    }
}

Sphynx::Core::Graphics::GL::GLUniform::GLUniform(void* data, ShaderDataType type) : Data(data), Type(type)
{
    
}
