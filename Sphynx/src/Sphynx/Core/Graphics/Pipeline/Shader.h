#pragma once
#include <string>
#include <any>
namespace Sphynx::Core::Graphics {
	enum class ShaderType : unsigned char {
		VertexShader,
		FragmentShader,//The Same as pixel shader
		PixelShader = FragmentShader,//The Same as fragment shader
		GeometryShader,
		//OpenGL names. add Ones for DX11/DX12 (For Future Compatibility)
		TessellationControlShader, //Optional, For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Control_Shader
		TessellationEvaluationShader //Mandatory for tessellation to be active,//For more detail : https://www.khronos.org/opengl/wiki/Tessellation_Evaluation_Shader
	};
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Double, Double2, Double3, Double4, Int, Int2, Int3, Int4, UInt, UInt2, UInt3, UInt4, Bool,
		Mat2x2, Mat2x3, Mat2x4, Mat3x2, Mat3x3, Mat3x4, Mat4x2, Mat4x3, Mat4x4
	};
	class Shader
	{
	protected:
		ShaderType Type;
	public:
		//Create The Shader With the given File.
		static Shader* Create(const char* code, ShaderType type);
		ShaderType GetShaderType() { return Type; };
		virtual bool IsValid() = 0;
		virtual ~Shader() = default;
	};

	typedef struct ShaderPack {
		Shader* Vert;
		Shader* Frag;
		Shader* TessEval;
		Shader* TessControl;
		Shader* Geom;

		void ReplaceShader(Shader* shader) {
			switch (shader->GetShaderType())
			{
			case Sphynx::Core::Graphics::ShaderType::VertexShader:
				Vert = shader;
				break;
			case Sphynx::Core::Graphics::ShaderType::FragmentShader:
				Frag = shader;
				break;
			case Sphynx::Core::Graphics::ShaderType::GeometryShader:
				Geom = shader;
				break;
			case Sphynx::Core::Graphics::ShaderType::TessellationControlShader:
				TessControl = shader;
				break;
			case Sphynx::Core::Graphics::ShaderType::TessellationEvaluationShader:
				TessEval = shader;
				break;
			}
		}
	}ShaderPack;
}