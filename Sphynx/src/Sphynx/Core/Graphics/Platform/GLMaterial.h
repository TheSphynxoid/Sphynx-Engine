#pragma once
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Platform/GLTexture.h"
#include <glm/glm.hpp>
#include <glad/glad.h>
#include <functional>
#pragma region Default Shader
#define DEF_VSHADER \
"#version 330 core\n"\
"layout(location = 0) in vec3 aPos;"\
"out vec4 vertexColor;"\
"void main()"\
"{"\
"	gl_Position = vec4(aPos, 1.0);"\
"	vertexColor = vec4(0.0, 0.0, 1.0, 1.0);"\
" }"
#define DEF_FSHADER \
"#version 330 core\n"\
"out vec4 FragColor;"\
"in vec4 vertexColor;"\
"void main()"\
"{"\
"	FragColor = vertexColor;"\
"}"
#pragma endregion

namespace Sphynx::Core::Graphics::GL {
    class GLMaterial;
    //Use The Set The Value For A uniform
    class UniformData final {
        std::function<void()> command;
    public:
        std::function<void()> GetCommand() { return command; };
        void SetUniform(GLMaterial* Mat);
        //Specialize This if you want to add more Type. 
        //To do so you need to simply set command with a function (or a lambda) that takes no parameter that gets called to set the uniform. Example:
        //UniformData(int loc, int Count, float* val) {
        //    command = [_val = val, _loc = loc, _c = Count]()->void { glUniform1fv(_loc, _c, _val); };
        //}
        template<typename T>
        UniformData(int loc, int Count, T val) {}
        template<>
        UniformData(int loc, int Count, float* val) {
            command = [_val = val, _loc = loc, _c = Count]()->void { glUniform1fv(_loc, _c, _val); };
        }
        template<>
        UniformData(int loc, int Count, int* val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform1iv(_loc, _c, _val); };
        }
        template<>
        UniformData(int loc, int Count, unsigned int* val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform1uiv(_loc, _c, _val); };
        }
        template<>
        UniformData(int loc, int Count, double* val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform1dv(_loc, _c, _val); };
        }
        template<>
        UniformData(int loc, int Count, glm::vec2 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform2fv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::dvec2 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform2dv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::ivec2 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform2iv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::uvec2 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform2uiv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::vec3 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform3fv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::dvec3 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform3dv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::ivec3 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform3iv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::uvec3 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform3uiv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::vec4 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform4fv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::dvec4 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform4dv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::ivec4 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform4iv(_loc, _c, &_val[0]); };
        }
        template<>
        UniformData(int loc, int Count, glm::uvec4 val) {
            command = [_val = val, _loc = loc, _c = Count]()->void {glUniform4uiv(_loc, _c, &_val[0]); };
        }
        template<typename Matrix>
        UniformData(int loc, Matrix val, int Count, bool Transpose) {
        }
        template<>
        UniformData(int loc, glm::mat2 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void 
            {glUniformMatrix2fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat2x3 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix2x3fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat2x4 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix2x4fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat3x2 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix3x2fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat3x3 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix3fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat3x4 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix3x4fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat4x2 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix4x2fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat4x3 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix4x3fv(_loc, _c, transpose, &_val[0][0]); };
        }
        template<>
        UniformData(int loc, glm::mat4x4 val, int Count, bool Transpose) {
            command = [_val = val, _loc = loc, transpose = Transpose, _c = Count]()->void
            {glUniformMatrix4fv(_loc, _c, transpose, &_val[0][0]); };
        }
        friend class GLMaterial;
    };
    class GLMaterial final : public Material
    {
    private:
        unsigned int ProgramId = 0;
        static GLMaterial* Bound;
        static GLMaterial DefaultMaterial;
        //Creates The GL Program.
        virtual void i_CreateMaterial(const ShaderPack& pack)override;
        //Gets Called By the Renderer To Ensure that this gets called after OpenGL Context is created.
        static GLMaterial CreateDefaultMaterial();
        void Release();
        //Texture Missing.
    public:
        GLMaterial() { ProgramId = 0; };
        GLMaterial(ShaderPack&& pack, GLTexture* _tex) : Material(pack, _tex) { i_CreateMaterial(pack); };
        //Binds The Program.
        virtual void Bind() override;
        GLMaterial(const GLMaterial& mat) = delete;
        GLMaterial& operator=(const GLMaterial& mat) = delete;
        GLMaterial(GLMaterial&& mat)noexcept;
        GLMaterial& operator=(GLMaterial&& mat)noexcept;
        //Checks Whether the Material Is Valid i.e. Succesfully Linked and created.
        virtual bool IsValid() override { return ProgramId; }
        ~GLMaterial();
        bool IsBound() { return Bound == this; };
        //Getters
        static GLMaterial* GetBoundMaterial() { return Bound; };
        //Returns the Default Material.
        static GLMaterial* GetDefaultMaterial() { return &DefaultMaterial; };
        int GetAttributeLocation(std::string name);
        int GetUniformLocation(std::string name);
        //Set The Uniform with the value given to UniformData.
        void SetUniformValue(UniformData data);
        friend class GLRenderer;
    };
}