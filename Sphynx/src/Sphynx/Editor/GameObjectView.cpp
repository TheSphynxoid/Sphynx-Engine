#include "pch.h"
#include "GameObjectView.h"
#include "Editor.h"
#include "imgui.h"
#include <string>
#include "Core/Factories/ComponentFactory.h"
#include "Camera.h"
//#include "Camera.h"
//#include "Core/Graphics/Pipeline/FrameBuffer.h"
//#include "Core/Graphics/Pipeline/Texture.h"

ImGuiID DockID = 0;
std::string Title;
static char name[124];
float TransformV[3];
float TransformR[3];
float TransformS[3];


Sphynx::Editor::GameObjectView::GameObjectView()
{
}

void Sphynx::Editor::GameObjectView::Draw()
{
	if (DockID == 0) {
		DockID = Core::Imgui::GetOverlayWindow<Editor>()->GetDockID();
	}
	ImGui::SetNextWindowDockID(DockID, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("GameObjectView", &IsOpen)) {
		if (CurrentGO) {
			ImGui::Text("Name:");
			ImGui::SameLine();
			if (ImGui::InputText("##Name", name, sizeof(name))) {
				CurrentGO->SetName(name);
			}
			ImGui::Separator();
			ImGui::Text("Transform");
			if (ImGui::DragFloat3("Position", TransformV, std::abs(ImGui::GetMouseDragDelta().x) / 10000)) {
				CurrentGO->GetTransform()->SetPosition({ TransformV[0],TransformV[1],TransformV[2] });
			}
			if (ImGui::DragFloat3("Rotation", TransformR, std::abs(ImGui::GetMouseDragDelta().x) / 10000)) {
				//CurrentGO->GetTransform()->Set({ TransformV[0],TransformV[1],TransformV[2] });
			}
			if (ImGui::DragFloat3("Scale", TransformS, std::abs(ImGui::GetMouseDragDelta().x) / 10000)) {
				//CurrentGO->GetTransform()->Set({ TransformV[0],TransformV[1],TransformV[2] });
			}
			ImGui::Separator();
			if (Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<Camera>(CurrentGO)) {
				
				ImGui::Separator();
			}
		}
	}
	ImGui::End();
}

void Sphynx::Editor::GameObjectView::SetGameObjectView(Sphynx::GameObject* go)
{
	CurrentGO = go;
	strcpy(name, CurrentGO->GetName());
	auto pos = go->GetTransform()->GetPosition();
	TransformV[0] = pos[0];
	TransformV[1] = pos[1];
	TransformV[2] = pos[2];
}
