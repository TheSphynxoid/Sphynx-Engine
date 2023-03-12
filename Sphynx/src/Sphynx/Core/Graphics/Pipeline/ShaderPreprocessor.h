#pragma once
#include <unordered_set>
#include "Shader.h"

namespace Sphynx::Core::Graphics {
	class ShaderPreprocessor
	{
	private:
		struct Definition {
			std::string Name;
			std::string value;
		};
		struct Token{
			std::string TokenString;
		};
		inline static std::string IncludePath;
		//inline static std::unordered_set<Token> TokenMap;
		inline static int ShaderVersion = 440;
		inline static bool IgnoreOutOfDate = true;
		inline static std::list<Definition> Defs;

		static std::string& ProcessBasic(std::string& code);
		static std::string& PreprocessVertexShader(std::string& code);
		static std::string& PreprocessFragmentShader(std::string& code);
	public:
		static void PreprocessShader(std::string& code, ShaderType type);
		static void SetGLShaderVersion(int version) { ShaderVersion = version; };
		static int GetGLShaderVersion() { return ShaderVersion; };
		static void RejectOutDatedShader(bool ignore) { IgnoreOutOfDate = ignore; };
		static std::list<Definition>& GetDefinition() { return Defs; };
		static void AddDefinition(std::string name, std::string val);
	};
}
