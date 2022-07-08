#include "pch.h"
#include "ResourceManager.h"
#include <fstream>
#include <sstream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Sphynx::Core::Graphics::Shader* Sphynx::ResourceManager::LoadShader(const char* path, Sphynx::Core::Graphics::ShaderType type)
{
	//Read File.
	std::fstream in;
	in.open(path, in.in);
	std::string glsl((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();
	auto code = glsl.c_str();
	auto shader = Sphynx::Core::Graphics::Shader::Create(code, type);
	in.close();
	glsl.clear();
	code = NULL;
	return shader;
}

Sphynx::Core::Graphics::Texture* Sphynx::ResourceManager::LoadTexture(const char* path, Core::Graphics::TextureType type, bool Compress)
{
	void* data;
	int x, y, bits;
	data = stbi_load(path, &x, &y, &bits, 0);
	Sphynx::Core::Graphics::TextureFormat format;
	switch (bits)
	{
	case 1:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRed;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::Red;
		}
		break;
	case 2:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRG;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RG;
		}
		break;
	case 3:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGB;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGB;
		}
		break;
	case 4:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGBA;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGBA;
		}
		break;
	default:
		if (Compress) {
			format = Sphynx::Core::Graphics::TextureFormat::CompressedRGBA;
		}
		else {
			format = Sphynx::Core::Graphics::TextureFormat::RGBA;
		}
		break;
	}
	auto Tex = Sphynx::Core::Graphics::Texture::Create(type, x, y, format, Sphynx::Core::Graphics::TextureDataFormat::UByte);
	Tex->SetData(data);
	return Tex;
}
