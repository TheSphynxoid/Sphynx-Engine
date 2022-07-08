#include "pch.h"
#include "Editor.h"
#include "imgui.h"
#include "Core/SceneManager.h"

Sphynx::Core::AboutWindow* About = new Sphynx::Core::AboutWindow();

Sphynx::Editor::Editor::Editor()
{
	//Sphynx::Core::Imgui::AddOverlayWindow(new SceneManagerView());
	About->Close();
	Sphynx::Core::Imgui::AddOverlayWindow(About);
}

void Sphynx::Editor::Editor::Draw()
{
	DockID = ImGui::DockSpaceOverViewport(0, ImGuiDockNodeFlags_PassthruCentralNode);
	ImGui::BeginMainMenuBar();
	if (ImGui::BeginMenu("Project")) {
		if (ImGui::MenuItem("New Project")) {}
		if(ImGui::MenuItem("Open Project")){}
		if(ImGui::MenuItem("Save Project")){}
		ImGui::Separator();
		if(ImGui::MenuItem("New Scene")){}
		if(ImGui::MenuItem("Open Scene")){}
		if (ImGui::MenuItem("Save Scene As...")){}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Edit")) {
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("Debug")) {
		if (ImGui::MenuItem("Debug Window")) {
			Sphynx::Core::Imgui::AddOverlayWindow(new Core::DebugWindow());
		}
		ImGui::EndMenu();
	}
	if (ImGui::BeginMenu("About")) {
		if (ImGui::MenuItem("About Sphynx Engine")) {}
		if (ImGui::MenuItem("About ImGui")) {
			About->Open();
		}
		if (ImGui::MenuItem("Licenses")) {}
		ImGui::EndMenu();
	}
	ImGui::EndMainMenuBar();
}
