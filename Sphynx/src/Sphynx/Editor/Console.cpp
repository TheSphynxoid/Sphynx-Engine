#include "pch.h"
#include "Console.h"
#include "Editor.h"
#include "imgui.h"
#include "Logger.h"

namespace Sphynx::Editor {
	//Because of the inablity to put this in DebugWindow,it's here.
	ImGuiTextBuffer     Buf;
	//Because of the inablity to put this in DebugWindow,it's here.
	ImGuiTextFilter     Filter;
	//Because of the inablity to put this in DebugWindow,it's here.
	ImVector<int>       LineOffsets;
	//For Consistancy's sake this going to be ImVector and here.
	ImVector<ImVec4>	Colors;

	ImVec4 GetLevelColor(spdlog::level::level_enum& lvl)
	{
		switch (lvl)
		{
		default:
			return ImVec4(0, 0, 0, 1);
		case spdlog::level::info:
			return ImVec4(0, 0, 1, 1);
		case spdlog::level::trace:
			return ImVec4(0.5f, 0.5f, 0, 1);
		case spdlog::level::err:
			return ImVec4(1, 0, 0, 1);
		case spdlog::level::warn:
			return ImVec4(0, 0.5f, 0.5f, 1);
		case spdlog::level::debug:
			return ImVec4(1, 1, 1, 1);
		}
	}

	void OnLogEvent(Sphynx::OnLog& e) {
		int old_size = Buf.size();
		Buf.appendfv(e.msg.c_str(), NULL);
		for (int new_size = Buf.size(); old_size < new_size; old_size++) {
			if (Buf[old_size] == '\n') {
				LineOffsets.push_back(old_size + 1);
				Colors.push_back(GetLevelColor(e.level));
			}
		}
	}

}


Sphynx::Editor::Console::Console()
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<OnLog>(OnLogEvent);
}

void Sphynx::Editor::Console::Draw()
{
	if (ImGui::Begin("Console", &IsOpen)) {
		ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
		const char* buf = Buf.begin();
		const char* buf_end = Buf.end();
		ImGuiListClipper clipper;
		clipper.Begin(LineOffsets.Size - 1);
		while (clipper.Step())
		{
			for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
			{
				const char* line_start = buf + LineOffsets[line_no];
				const char* line_end = (line_no + 1 < LineOffsets.Size) ? (buf + LineOffsets[line_no + 1] - 1) : buf_end;
				ImGui::PushStyleColor(ImGuiCol_::ImGuiCol_Text, Colors[line_no]);
				ImGui::TextUnformatted(line_start, line_end);
				ImGui::PopStyleColor();
			}
		}
		clipper.End();
		ImGui::PopStyleVar();
		ImGui::EndChild();
	}
	ImGui::End();
}
