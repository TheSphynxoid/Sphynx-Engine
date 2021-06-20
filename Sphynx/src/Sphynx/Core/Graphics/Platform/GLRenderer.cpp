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
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sphynx::Core::Graphics::GL::GLRenderer::Start(IWindow* app)
{
}

void Sphynx::Core::Graphics::GL::GLRenderer::Render()
{
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	for(auto pair : RenderQueue){
		// pair.second->front().mat->Bind();
		glUseProgram(pair.first);
		for(auto rend : *pair.second){
			rend.mesh->Bind();
			GLMesh* mesh = (GLMesh*)rend.mesh;
			if(mesh->HasIndexArray()){
				glDrawElements(GL_TRIANGLES,mesh->GetIndexArraySize(),GL_INT,0);
			}else{
				glDrawArrays(GL_TRIANGLES,0,mesh->GetVertexArraySize());
			}
		}
	}
}

void Sphynx::Core::Graphics::GL::GLRenderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Sphynx::Core::Graphics::GL::GLRenderer::OnSubmit(RenderObject rend)
{
	//I think this is sorted.
	auto l = RenderQueue[((GLMaterial*)rend.mat)->ProgramId];
	if(l == nullptr){
		l = new RenderObjectList();
	}
	l->push_back(rend);
}

void Sphynx::Core::Graphics::GL::GLRenderer::SetClearColor(Vec4 color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}
