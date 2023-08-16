#include "pch.h"
#include "GameObjectView.h"
#include "Editor.h"
#include "imgui.h"
#include <string>
#include "Core/Factories/ComponentFactory.h"
#include "Camera.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include "Core/MeshRenderer.h"

ImGuiID DockID = 0;
std::string Title;
static char name[124];
float TransformV[3];
float TransformR[3];
float TransformS[3];
Sphynx::MeshRenderer* mr = nullptr;


Sphynx::Editor::GameObjectView::GameObjectView()
{
}

void UpdateCoords(Sphynx::GameObject* go) {
	auto pos = go->GetTransform()->GetPosition();
	auto scale = go->GetTransform()->GetScale();
	auto rot = glm::degrees(glm::eulerAngles(go->GetTransform()->GetRotation()));
	
	memcpy_s(TransformV, sizeof(TransformV), &pos[0], sizeof(pos));
	memcpy_s(TransformS, sizeof(TransformS), &scale[0], sizeof(scale));
	memcpy_s(TransformR, sizeof(TransformR), &rot[0], sizeof(rot));
}

void Sphynx::Editor::GameObjectView::Draw()
{
	if (DockID == 0) {
		DockID = Core::Imgui::GetOverlayWindow<Editor>()->GetDockID();
	}
	ImGui::SetNextWindowDockID(DockID, ImGuiCond_FirstUseEver);
	if (ImGui::Begin("GameObjectView", &IsOpen)) {
		if (CurrentGO) {
			UpdateCoords(CurrentGO);
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
				CurrentGO->GetTransform()->SetRotation({ TransformR[0],TransformR[1],TransformR[2] });
			}
			if (ImGui::DragFloat3("Scale", TransformS, std::abs(ImGui::GetMouseDragDelta().x) / 10000)) {
				CurrentGO->GetTransform()->SetScale({ TransformS[0],TransformS[1],TransformS[2] });
			}
			ImGui::Separator();
			if (Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<Camera>(CurrentGO)) {
				ImGui::Text("Camera");
				ImGui::Separator();
			}
			if (Core::Internal::ComponentFactory::ComponentHelper::IsComponentInGameObject<MeshRenderer>(CurrentGO)) {
				ImGui::Text("Mesh Renderer");
				if (ImGui::TreeNode("Mesh")) {
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Material")) {
					if (ImGui::TreeNode("Textures {0}", std::to_string(mr->GetMaterial()->GetTextureCount()).c_str())) {
					}
					ImGui::TreePop();
				}
				ImGui::Separator();
			}
		}
	}
	ImGui::End();
}

void Sphynx::Editor::GameObjectView::SetGameObjectView(Sphynx::GameObject* go)
{
	CurrentGO = go;
	mr = go->GetComponent<MeshRenderer>();
	strcpy(name, CurrentGO->GetName());
	UpdateCoords(go);
}
