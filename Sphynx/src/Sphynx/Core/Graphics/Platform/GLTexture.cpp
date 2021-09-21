#include "pch.h"
#include "GLTexture.h"
#include "glad/glad.h"

void Sphynx::Core::Graphics::GL::GLTexture::Release()
{
	glDeleteTextures(1, &TextureID);
	TextureID = 0;
}
