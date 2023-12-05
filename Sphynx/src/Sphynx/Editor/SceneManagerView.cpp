#include "pch.h"
#include "SceneManagerView.h"
#include "imgui.h"
#include "Core/SceneManager.h"
#include "Scene.h"
#include "Application.h"
#include "Core/Graphics/Window.h"
#include <charconv>
#include "Editor/Editor.h"
#include "Editor/GameObjectView.h"

ImGuiID dockid = 0;

Sphynx::Editor::GameObjectView* goView;

Sphynx::Editor::SceneManagerView::SceneManagerView()
{
}

void Sphynx::Editor::SceneManagerView::Draw()
{
	if (dockid == 0) {
		dockid = Sphynx::Core::Imgui::GetOverlayWindow<Editor>()->GetDockID();
		goView = Sphynx::Core::Imgui::GetOverlayWindow<GameObjectView>();
	}
	ImGui::SetNextWindowDockID(dockid, ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowSize(ImVec2(150, GetMainWindow()->GetHeight() - 15), ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Scene Manager", &IsOpen, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar)) {
		if (ImGui::BeginMenuBar()) {
			if (ImGui::Button("Create Object", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 35))) {
				Core::SceneManager::GetScene().AddGameObject(GameObject::CreatePrimitive(Primitives::Cube));
			}
			ImGui::EndMenuBar();
		}
		if (ImGui::BeginListBox("##GameObject", ImVec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight() - 35))) {
			static int selected = -1;
			int i = 0;
			for (auto& go : Sphynx::Core::SceneManager::GetScene().GetGameObjects()) {
				if (ImGui::Selectable((std::string(go->GetName()) + "##" + std::to_string(go->GetID())).c_str()
					, selected == i, go->IsActive() ? ImGuiSelectableFlags_None : ImGuiSelectableFlags_Disabled)) {
					selected = i;
					goView->SetGameObjectView(go);
				}
				i++;
			}
			ImGui::EndListBox();
		}
	}
	ImGui::End();
}
