#include "pch.h"
#include "GLRenderer.h"
#define GLFW_INCLUDE_NONE
//Before IMGUI.
#include <GLFW/glfw3.h>
//IMGUI gets loaded here.
#include "GLWindow.h"
#include "glad/glad.h"
//#define GLFW_EXPOSE_NATIVE_WIN32
//#include "GLFW/glfw3native.h"
#include "GLMaterial.h"
#include "GLMesh.h"
#include "GLShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma region Default Shader
#define DEF_VSHADER \
"#version 330 core\
layout(location = 0) in vec3 aPos;\
out vec4 vertexColor;\
void main()\
{\
	gl_Position = vec4(aPos, 1.0);\
	vertexColor = vec4(0.5, 0.0, 0.0, 1.0);\
 }"
#define DEF_FSHADER \
"#version 330 core\
out vec4 FragColor;\
in vec4 vertexColor;\
void main()\
{\
	FragColor = vertexColor;\
}"
#pragma endregion

void Sphynx::Core::Graphics::GL::GLRenderer::RendererResizeEvent(Events::OnWindowResize& e)
{
	glViewport(0, 0, e.Width, e.Height);
}

void Sphynx::Core::Graphics::GL::GLRenderer::Start(IWindow* app)
{
	app->GetEventSystem()->Subscribe(this, &GLRenderer::RendererResizeEvent);
	if (!DefaultsSet) {
		DefaultsSet = true;
		DefaultVertexShader = new GL::GLShader();
		DefaultVertexShader->CreateFromCode(DEF_VSHADER, ShaderType::VertexShader);
		DefaultFragmentShader = new GL::GLShader;
		DefaultFragmentShader->CreateFromCode(DEF_FSHADER, ShaderType::FragmentShader);
		ShaderPack sp = ShaderPack(DefaultVertexShader, DefaultFragmentShader, nullptr, nullptr);
		DefaultMaterial = new GLMaterial(sp, nullptr);
		DefaultRenderObject = RenderObject(nullptr, DefaultMaterial, { 0,0,0 }, { 0,0,0,0 });
	}
}

void Sphynx::Core::Graphics::GL::GLRenderer::Render()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	for (auto pair : RenderQueue) {
		// pair.second->front().mat->Bind();
		glUseProgram(pair.first);
		for (auto rend : *pair.second) {
			rend.mesh->Bind();
			GLMesh* mesh = (GLMesh*)rend.mesh;
			if (mesh->HasIndexArray()) {
				glDrawElements(GL_TRIANGLES, mesh->GetIndexArraySize(), GL_INT, 0);
			}
			else {
				glDrawArrays(GL_TRIANGLES, 0, mesh->GetVertexArraySize());
			}
		}
	}
	//Clearing Queue for new loop. things will get messy with multi-threading.
	RenderQueue.clear();
}

void Sphynx::Core::Graphics::GL::GLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Sphynx::Core::Graphics::GL::GLRenderer::OnSubmit(RenderObject rend)
{
	if (!rend.mat) {
		rend.mat = DefaultMaterial;
	}
	//I think this is sorted.
	auto l = RenderQueue[((GLMaterial*)rend.mat)->ProgramId];
	if (l == nullptr) {
		l = new RenderObjectList();
	}
	l->push_back(rend);
}

void Sphynx::Core::Graphics::GL::GLRenderer::SetClearColor(Vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}
