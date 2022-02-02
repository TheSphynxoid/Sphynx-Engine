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
	//TODO: This Wrong. Find a way to add multi camera support and handle scene changes in the future.
}

void Sphynx::Editor::EditorViewport::Draw()
{
	if (ViewDockID == 0) {
		ViewDockID = Sphynx::Core::Imgui::GetOverlayWindow<Editor>()->GetDockID();
		SceneManager::GetScene().GetPrimaryCamera()->SetFrameBuffer(FrameBuffer::Create(100, 100,
			{ Texture::Create(TextureType::Texture2D,100,100,TextureFormat::RGBA,TextureDataFormat::UByte),
			Texture::Create(TextureType::Texture2D, 100,100, TextureFormat::Depth24_Stencil8,
			TextureDataFormat::UInt_24_8) }));
		ResizeFlag = true;
		//SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->SetClearColor({ 1,1,1,1 });
	}
	ImGui::SetNextWindowDockID(ViewDockID, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("Viewport", &IsOpen, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar)) {
		if (ResizeFlag) {
			ResizeFlag = false;
			Sphynx::Core::SceneManager::GetScene().GetPrimaryCamera()->SetViewport({ 0,0,(int)ImGui::GetWindowWidth(),(int)ImGui::GetWindowHeight() });
		}
		ImGui::Image(SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->GetColorAttachment(0)->GetNativeID(),
			{ (float)SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->GetWidth(), 
				(float)SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->GetHeight() });
        static int corner = 1;
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
			ImVec2 work_pos = ImGui::GetWindowPos(); // Use work area to avoid menu-bar/task-bar, if any!
            ImVec2 work_size = ImGui::GetWindowSize();
            ImVec2 window_pos, window_pos_pivot;
            window_pos.x = (work_pos.x + work_size.x - 10.0f);
            window_pos.y = (work_pos.y + 25.0f);
            window_pos_pivot.x = 1.0f;
            window_pos_pivot.y = 0.0f;
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        }
        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        if (ImGui::Begin("Viewport Details", &IsOpen, window_flags))
        {
			auto fb = SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer()->GetColorAttachment(0);
			ImGui::Text("Framebuffer Size:%i,%i", fb->GetWidth(), fb->GetHeight());
			ImGui::Text("Viewport Size:%i,%i", (int)ImGui::GetWindowWidth(), (int)ImGui::GetWindowHeight());
			ImGui::Text("FPS:%i", (int)(1 / Time::GetDeltaTime()));
        }
        ImGui::End();
	}
	ImGui::End();
}
