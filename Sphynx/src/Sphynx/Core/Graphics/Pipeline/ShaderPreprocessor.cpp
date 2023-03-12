#include "pch.h"
#include "ShaderPreprocessor.h"

std::string& Sphynx::Core::Graphics::ShaderPreprocessor::ProcessBasic(std::string& code)
{
	auto include = code.find("#include", 0);
	while (include != code.npos) {
		return code;
	}
}

void Sphynx::Core::Graphics::ShaderPreprocessor::PreprocessShader(std::string& code, ShaderType type)
{
	auto PCode = ProcessBasic(code);
	switch (type)
	{
	case Sphynx::Core::Graphics::ShaderType::VertexShader:
		PCode = PreprocessVertexShader(PCode);
		break;
	case Sphynx::Core::Graphics::ShaderType::FragmentShader:
		PCode = PreprocessFragmentShader(PCode);
		break;
	case Sphynx::Core::Graphics::ShaderType::GeometryShader:
		break;
	case Sphynx::Core::Graphics::ShaderType::TessellationControlShader:
		break;
	case Sphynx::Core::Graphics::ShaderType::TessellationEvaluationShader:
		break;
	default:
		break;
	}
	code = PCode;
}
