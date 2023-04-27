#include "pch.h"
#include "TextureDebugger.h"
#include "imgui.h"

void Sphynx::Core::TextureDebugger::Draw()
{
	if (ImGui::Begin("Texture Debugger", &IsOpen, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static int LineCount = 0;
		for (auto& [path, tex] : loadedTex) {
			if (LineCount < 4)ImGui::SameLine();
			else LineCount = 0;
			ImGui::ImageButton(tex->GetNativeID(), { 125,125 });
			++LineCount;
		}
		for (auto& tex : SpecialTex) {
			if (LineCount < 4)ImGui::SameLine();
			else LineCount = 0;
			ImGui::ImageButton(tex->GetNativeID(), { 125,125 });
			++LineCount;
		}
		LineCount = 0;
	}
	ImGui::End();
}
