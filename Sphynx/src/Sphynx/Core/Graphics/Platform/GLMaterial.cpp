#include "pch.h"
#include "GLMaterial.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "glad/glad.h"

using namespace Sphynx::Core::Graphics::GL;

GLMaterial::GLMaterial(ShaderPack shaders,Texture* texture) : Material(shaders, texture){
    ProgramId = glCreateProgram();
    glUseProgram(ProgramId);
    glAttachShader(ProgramId,((GLShader*)shaders.Vert)->id);
    glAttachShader(ProgramId,((GLShader*)shaders.Frag)->id);
    glDeleteShader(((GLShader*)shaders.Vert)->id);
    glDeleteShader(((GLShader*)shaders.Frag)->id);
    if(shaders.Geom != NULL){
        glAttachShader(ProgramId,((GLShader*)shaders.Geom)->id);
        glDeleteShader(((GLShader*)shaders.Geom)->id);
    }
    if(shaders.Tess != NULL){
        glAttachShader(ProgramId,((GLShader*)shaders.Tess)->id);
        glDeleteShader(((GLShader*)shaders.Tess)->id);
    }
}

void GLMaterial::Bind(){
    glUseProgram(ProgramId);
}