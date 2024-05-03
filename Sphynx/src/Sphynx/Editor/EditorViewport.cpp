#include "pch.h"
#define Editor_Core
#include "EditorViewport.h"
#include "Editor/Editor.h"
#include "imgui.h"
#include "Core/Graphics/Window.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/SceneManager.h"
#include "Events/Event.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include "Core/Graphics/Pipeline/Texture.h"

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

ImGuiID ViewDockID = 0;

void Sphynx::Editor::EditorViewport::OnResize(Events::OnWindowResize& e)
{
	ResizeFlag = true;
}

Sphynx::Editor::EditorViewport::EditorViewport() : viewport()
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<EditorViewport, Events::OnWindowResize>(this, &EditorViewport::OnResize);
}

void Sphynx::Editor::EditorViewport::Draw()
{
	static bool StatsToggle = true;
	static ImVec2 WinSize;
	if ((Input::IsKeyPressed(Keys::F3) && (Input::GetMods(Keys::F3) == Mods::Shift)))
	{
		StatsToggle != StatsToggle;
	}
	auto Cam = SceneManager::GetCurrentScene().GetPrimaryCamera();
	if (ViewDockID == 0) {
		ViewDockID = Sphynx::Core::Imgui::GetOverlayWindow<Editor>()->GetDockID();
		Cam->SetFrameBuffer(FrameBuffer::Create(
			{ Texture::Create(TextureType::Texture2D,100,100,0,TextureFormat::RGBA,TextureDataFormat::UByte),
			Texture::Create(TextureType::Texture2D, 100,100,0, TextureFormat::Depth24_Stencil8,
			TextureDataFormat::UInt_24_8) }));
		ResizeFlag = true;
		
	}
	ImGui::SetNextWindowDockID(ViewDockID, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Viewport", &IsOpen, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar
		| ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoInputs)) {
		ImVec2 CurrSize = ImGui::GetContentRegionAvail();
		if (CurrSize.x != WinSize.x || CurrSize.y != WinSize.y) {
			ResizeFlag = false;
			WinSize = CurrSize;
			Cam->SetViewport(
				{ 0,0,(int)WinSize.x,(int)WinSize.y });
		}
		const auto& tex = Cam->GetFrameBuffer()->GetColorAttachment(0);
		ImGui::Image(tex->GetNativeID(),
			{ (float)tex->GetWidth(),
				(float)tex->GetHeight() }, { 0,1 }, { 1,0 });
		if (StatsToggle) {
			//Render info widget
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			ImVec2 work_pos = ImGui::GetWindowPos(); // Use work area to avoid menu-bar/task-bar, if any!
			ImVec2 work_size = ImGui::GetWindowSize();
			ImVec2 window_pos, window_pos_pivot;
			window_pos.x = (work_pos.x + work_size.x - 10.0f);
			window_pos.y = (work_pos.y + 25.0f);
			window_pos_pivot.x = 1.0f;
			window_pos_pivot.y = 0.0f;
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("Viewport Details", &IsOpen, window_flags))
			{
				auto fb = SceneManager::GetCurrentScene().GetPrimaryCamera()->GetFrameBuffer()->GetColorAttachment(0);
				ImGui::Text("Framebuffer Size:%i,%i", fb->GetWidth(), fb->GetHeight());
				ImGui::Text("Viewport Size:%i,%i", (int)ImGui::GetWindowWidth(), (int)ImGui::GetWindowHeight());
				ImGui::Text("FPS:%i", (int)(1 / Time::GetDeltaTime()));
			}
			//End Render info widget 
			ImGui::End();
		}
	}
	ImGui::End();
}
