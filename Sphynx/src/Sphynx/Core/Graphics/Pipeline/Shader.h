#pragma once
#include <string>
namespace Sphynx::Core::Graphics {
	class Shader
	{
	public:
		virtual void Load(std::string path) = 0;
		virtual ~Shader() = 0;
	};
	//class GeometryShader : public Shader
	//{
	//};
	////TCS and TES
	//class TessellationShader : public Shader{};
	//class VertexShader : public Shader{};
	//class FragmentShader : public Shader{};

	struct ShaderPack {
		Shader* Vert;
		Shader* Frag;
		Shader* Tess;
		Shader* Geom;

		ShaderPack(Shader* vert, Shader* frag, Shader* tess, Shader* geom) {
			Vert = vert;
			Frag = frag;
			Tess = tess;
			Geom = geom;
		}
	};
}