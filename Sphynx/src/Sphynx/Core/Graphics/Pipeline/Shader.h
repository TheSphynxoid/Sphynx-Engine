#pragma once
#include <string>
#include <any>
namespace Sphynx::Core::Graphics {
	enum class ShaderType : unsigned char {
		VertexShader,
		FragmentShader,//The Same as pixel shader
		PixelShader = FragmentShader,//The Same as fragment shader
		GeometryShader,
		TessellationControlShader, //Optional, For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Control_Shader
		TessellationEvaluationShader //Mandatory for tessellation to be active,//For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Evaluation_Shader
	};
	class Shader
	{
	protected:
		ShaderType Type;
		virtual void Load(std::string path, ShaderType type) = 0;
	public:
		//Create The Shader With the given File.
		void CreateShader(std::string path, ShaderType type) {
			Type = type;
			Load(path, type);
		}
		ShaderType GetShaderType() { return Type; };
		virtual bool IsValid() = 0;
		virtual ~Shader() = default;
	};

	typedef struct ShaderPack {
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
	}ShaderPack;
}