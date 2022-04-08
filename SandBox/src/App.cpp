#include <Sphynx.h>
#include <Sphynx/Events/ApplicationEvents.h>
#include <iostream>
#include <string>
#include <Sphynx/Core/Graphics/Platform/GLWindow.h>
#include <Sphynx/Core/Transform.h>
#include <Sphynx/Camera.h>
#include <Editor/Editor.h>


using namespace Sphynx;

class SandBox : public Sphynx::Application
{
private:
public:
	SandBox() {
		//Sphynx::Core::Bounds b = { 512, 384 };
		//this->GetMainWindow()->Resize(b.Width, b.Height);
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::Editor());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::SceneManagerView());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::EditorViewport());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::GameObjectView());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::Console());
	}
	void Start() {
	}
	void Update() {
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	return sandbox;
}