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

using namespace Sphynx;
using namespace Sphynx::Core;

class SandBox : public Sphynx::Application
{
private:
	GameObject Card;
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
		Card = GameObject::CreatePrimitive(Primitives::Cube, "Cube");
		Card.GetComponent<MeshRenderer>()->GetMaterial()->AddTexture(ResourceManager::LoadTexture("wall.jpg",
			Sphynx::Core::Graphics::TextureType::Texture2D, true));
		//auto font = ResourceManager::LoadFont("fonts/atwriter.ttf");
		Sphynx::Core::SceneManager::GetScene().AddGameObject(&Card);
	}
	void Update() {	
		if (Input::IsKeyPressed(Keys::Up)) {
			Card.GetTransform()->Translate(glm::vec3(0, 1, 0));
		}
		if (Input::IsKeyPressed(Keys::Down)) {
			Card.GetTransform()->Translate(glm::vec3(0, -1, 0));
		}
		if (Input::IsKeyPressed(Keys::Enter)) {
			Card.GetTransform()->SetPosition(glm::vec3(GetMainWindow()->GetWidth() / 2, GetMainWindow()->GetHeight() / 2, 0));
		}
	}
	~SandBox() {

	}
};

//The Main Entry Point for clients
Sphynx::Application* Sphynx::CreateApplication() {
	auto sandbox = new SandBox();
	sandbox->GetMainWindow()->ChangeTitle("\"Home\"");
	return sandbox;
}