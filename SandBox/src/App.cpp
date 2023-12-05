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
#include <Core/Scripting/ScriptComponent.h>

using namespace Sphynx;
using namespace Sphynx::Core;

class SandBox : public Sphynx::Application
{
private:
	GameObject* TextTest;
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
#elif RELEASE

#endif
	}
	void Start() {
		TextTest = GameObject::CreatePrimitive(Primitives::Cube);
		auto sc = TextTest->AddComponent<ScriptComponent>();
		sc->AddScript("TestComponent");
		sc->AddScript("TestComponent2");
		Sphynx::Core::SceneManager::GetScene().AddGameObject(TextTest);
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

