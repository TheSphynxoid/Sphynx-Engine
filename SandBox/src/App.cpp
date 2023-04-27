#include <Sphynx.h>
#include <iostream>
#include <string>
#ifdef DEBUG
#include <Sphynx/Editor/Editor.h>
#endif
#include <Sphynx/Core/SpriteRenderer.h>
#include <Sphynx/Core/SceneManager.h>
#include <Sphynx/Core/Graphics/Pipeline/Texture.h>
#include <Sphynx/ResourceManager.h>
#include <Editor/TextureDebugger.h>
#include <Core/TextRenderer.h>

using namespace Sphynx;
using namespace Sphynx::Core;

class SandBox : public Sphynx::Application
{
private:
	GameObject TextTest;
public:
	SandBox() {
		//Sphynx::Core::Bounds b = { 512, 384 };
		//this->GetMainWindow()->Resize(b.Width, b.Height);
#ifdef DEBUG
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::Editor());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::SceneManagerView());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::EditorViewport());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::GameObjectView());
		Sphynx::Core::Imgui::AddOverlayWindow(new Editor::Console());
		//GetApplication()
#elif RELEASE

#endif
	}
	void Start() {
		//auto font = ResourceManager::LoadFont("assets/fonts/atwriter.ttf",24);
		//auto tr = TextTest.AddComponent<Sphynx::Core::TextRenderer>(font, "Cringe");
		TextTest = GameObject::CreatePrimitive(Primitives::Cube, "Rayen");
		Sphynx::Core::SceneManager::GetScene().AddGameObject(&TextTest);
	}
	void Update() {	
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	sandbox->GetMainWindow()->ChangeTitle("Testing Grounds");
	return sandbox;
}

