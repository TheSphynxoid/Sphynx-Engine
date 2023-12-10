#include "pch.h"
#include "Imgui.h"
#pragma region ImGui Includes
#include <imgui.h>
#ifdef IMGUI_GLFW && !defined(IMGUI_DX11)
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#elif defined(IMGUI_DX11) && !defined(IMGUI_GLFW)
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx11.h"
#else
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "GLFW/glfw3.h"
#endif
#pragma endregion
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Events/Event.h"
#include "Logger.h"
#include <charconv>
#include "Platform/GLWindow.h"
#include "Core/SceneManager.h"
#include "Scene.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include "Core/Graphics/Pipeline/Texture.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#include <array>

using namespace Sphynx::Events;

ImGuiKey ImGui_ImplGlfw_KeyToImGuiKey(int key)
{
	switch (key)
	{
	case GLFW_KEY_TAB: return ImGuiKey_Tab;
	case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
	case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
	case GLFW_KEY_UP: return ImGuiKey_UpArrow;
	case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
	case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
	case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
	case GLFW_KEY_HOME: return ImGuiKey_Home;
	case GLFW_KEY_END: return ImGuiKey_End;
	case GLFW_KEY_INSERT: return ImGuiKey_Insert;
	case GLFW_KEY_DELETE: return ImGuiKey_Delete;
	case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
	case GLFW_KEY_SPACE: return ImGuiKey_Space;
	case GLFW_KEY_ENTER: return ImGuiKey_Enter;
	case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
	case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
	case GLFW_KEY_COMMA: return ImGuiKey_Comma;
	case GLFW_KEY_MINUS: return ImGuiKey_Minus;
	case GLFW_KEY_PERIOD: return ImGuiKey_Period;
	case GLFW_KEY_SLASH: return ImGuiKey_Slash;
	case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
	case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
	case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
	case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
	case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
	case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
	case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
	case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
	case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
	case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
	case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
	case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
	case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
	case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
	case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
	case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
	case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
	case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
	case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
	case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
	case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
	case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
	case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
	case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
	case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
	case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
	case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
	case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
	case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
	case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
	case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
	case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
	case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_RightShift;
	case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
	case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
	case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
	case GLFW_KEY_MENU: return ImGuiKey_Menu;
	case GLFW_KEY_0: return ImGuiKey_0;
	case GLFW_KEY_1: return ImGuiKey_1;
	case GLFW_KEY_2: return ImGuiKey_2;
	case GLFW_KEY_3: return ImGuiKey_3;
	case GLFW_KEY_4: return ImGuiKey_4;
	case GLFW_KEY_5: return ImGuiKey_5;
	case GLFW_KEY_6: return ImGuiKey_6;
	case GLFW_KEY_7: return ImGuiKey_7;
	case GLFW_KEY_8: return ImGuiKey_8;
	case GLFW_KEY_9: return ImGuiKey_9;
	case GLFW_KEY_A: return ImGuiKey_A;
	case GLFW_KEY_B: return ImGuiKey_B;
	case GLFW_KEY_C: return ImGuiKey_C;
	case GLFW_KEY_D: return ImGuiKey_D;
	case GLFW_KEY_E: return ImGuiKey_E;
	case GLFW_KEY_F: return ImGuiKey_F;
	case GLFW_KEY_G: return ImGuiKey_G;
	case GLFW_KEY_H: return ImGuiKey_H;
	case GLFW_KEY_I: return ImGuiKey_I;
	case GLFW_KEY_J: return ImGuiKey_J;
	case GLFW_KEY_K: return ImGuiKey_K;
	case GLFW_KEY_L: return ImGuiKey_L;
	case GLFW_KEY_M: return ImGuiKey_M;
	case GLFW_KEY_N: return ImGuiKey_N;
	case GLFW_KEY_O: return ImGuiKey_O;
	case GLFW_KEY_P: return ImGuiKey_P;
	case GLFW_KEY_Q: return ImGuiKey_Q;
	case GLFW_KEY_R: return ImGuiKey_R;
	case GLFW_KEY_S: return ImGuiKey_S;
	case GLFW_KEY_T: return ImGuiKey_T;
	case GLFW_KEY_U: return ImGuiKey_U;
	case GLFW_KEY_V: return ImGuiKey_V;
	case GLFW_KEY_W: return ImGuiKey_W;
	case GLFW_KEY_X: return ImGuiKey_X;
	case GLFW_KEY_Y: return ImGuiKey_Y;
	case GLFW_KEY_Z: return ImGuiKey_Z;
	case GLFW_KEY_F1: return ImGuiKey_F1;
	case GLFW_KEY_F2: return ImGuiKey_F2;
	case GLFW_KEY_F3: return ImGuiKey_F3;
	case GLFW_KEY_F4: return ImGuiKey_F4;
	case GLFW_KEY_F5: return ImGuiKey_F5;
	case GLFW_KEY_F6: return ImGuiKey_F6;
	case GLFW_KEY_F7: return ImGuiKey_F7;
	case GLFW_KEY_F8: return ImGuiKey_F8;
	case GLFW_KEY_F9: return ImGuiKey_F9;
	case GLFW_KEY_F10: return ImGuiKey_F10;
	case GLFW_KEY_F11: return ImGuiKey_F11;
	case GLFW_KEY_F12: return ImGuiKey_F12;
	default: return ImGuiKey_None;
	}
}


void Sphynx::Core::Imgui::ImGuiOnWindowShutdown(Sphynx::Events::OnWindowClose& e) {
	Shutdown();
}

bool Sphynx::Core::Imgui::HandleKey(Keys key, int scan, Action action, Mods mod)
{
	auto& io = ImGui::GetIO();
	io.AddKeyEvent(ImGui_ImplGlfw_KeyToImGuiKey((int)key), action == Action::Pressed);
	return io.WantCaptureKeyboard;
}

void Sphynx::Core::Imgui::HandleChar(unsigned int codepoint)
{
	auto& io = ImGui::GetIO();
	io.AddInputCharacter(codepoint);
}

void Sphynx::Core::Imgui::HandleCursorPosition(glm::vec2 pos)
{
	auto& io = ImGui::GetIO();
	io.AddMousePosEvent(pos.x, pos.y);
}

bool Sphynx::Core::Imgui::HandleMouse(MouseButton button, Action action, Mods mod)
{
	auto& io = ImGui::GetIO();
	io.AddMouseButtonEvent((int)button, action == Action::Pressed);
	return io.WantCaptureMouse;
}

bool Sphynx::Core::Imgui::HandleScroll(double Xoffset, double Yoffset)
{
	auto& io = ImGui::GetIO();
	io.AddMouseWheelEvent((float)Xoffset, (float)Yoffset);
	return io.WantCaptureMouse;
}

void Sphynx::Core::Imgui::HandleFocus(OnWindowFocus& e)
{
	auto& io = ImGui::GetIO();
	io.AddFocusEvent(e.IsFocused());
}

void Sphynx::Core::Imgui::Start()
{
	App = GetApplication();
	window = GetMainWindow();
	window->GetEventSystem()->Subscribe<Events::OnWindowClose>(&Imgui::ImGuiOnWindowShutdown);
#ifdef IMGUI_DX11
	Imgui_ImplDX11_Init();
#else
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Text] = ImVec4(1.00f, 0.56f, 0.43f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.06f, 0.94f);
	colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_PopupBg] = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
	colors[ImGuiCol_Border] = ImVec4(1.00f, 1.00f, 1.00f, 0.12f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 0.04f, 0.25f, 1.00f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 0.19f, 0.48f, 1.00f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.04f, 0.31f, 0.51f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.03f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.11f, 0.07f, 0.27f, 0.39f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.70f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.69f, 1.00f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(1.00f, 0.58f, 1.00f, 1.00f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.24f, 0.52f, 0.88f, 1.00f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
	colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.31f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_Separator] = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
	colors[ImGuiCol_SeparatorHovered] = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
	colors[ImGuiCol_SeparatorActive] = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
	colors[ImGuiCol_Tab] = ImVec4(0.18f, 0.35f, 0.58f, 0.86f);
	colors[ImGuiCol_TabHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
	colors[ImGuiCol_TabActive] = ImVec4(0.20f, 0.41f, 0.68f, 1.00f);
	colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
	colors[ImGuiCol_DockingPreview] = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
	colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
	colors[ImGuiCol_PlotLines] = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
	colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
	colors[ImGuiCol_PlotHistogram] = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
	colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
	colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
	colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
	colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
	colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
	colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	//ImGui::StyleColorsDark();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)window->GetNativePointer(), false);
	Input::InstallKeyCallback(&HandleKey);
	Input::InstallMouseCallback(&HandleMouse);
	Input::InstallScrollCallback(&HandleScroll);
	Input::InstallCursorPositionCallback(&HandleCursorPosition);
	Input::InstallCharCallback(&HandleChar);
	GlobalEventSystem::GetInstance()->Subscribe<Events::OnWindowFocus>(&HandleFocus);
	ImGui_ImplOpenGL3_Init("#version 430 core");
#endif
}

void Sphynx::Core::Imgui::OnOverlayUpdate()
{
	if (Overlays.empty())return;
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	for (auto& w : Overlays) {
		if (w->IsOpen) {
			w->Draw();
			//IgnoreMouse = false;
		}
	}
	ImGui::EndFrame();
	ImGui::UpdatePlatformWindows();
	ImGui::Render();
	ImGui::GetDrawData();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

std::list<const char*> Sphynx::Core::Imgui::GetTypeInfoList()
{
	return Names;
}

void Sphynx::Core::Imgui::AddOverlayWindow(IOverlayWindow* Window)
{
	Overlays.push_back(Window);
	Names.push_back(typeid(*Window).name());
}

void Sphynx::Core::Imgui::RemoveOverlayWindow(IOverlayWindow* Window)
{
	Overlays.remove(Window);
	Window->Delete();
}

int Sphynx::Core::Imgui::GetNumberOfWindows()
{
	return Overlays.size();
}

void Sphynx::Core::Imgui::Shutdown()
{
	for (auto& w : Overlays) {
		if (w->IsOpen == false) {
			RemoveOverlayWindow(w);
			break;
		}
	}
	ImGui_ImplGlfw_Shutdown();
	ImGui_ImplOpenGL3_Shutdown();	
}

void Sphynx::Core::DemoWindow::Draw()
{
	ImGui::ShowDemoWindow(&IsOpen);
}

void Sphynx::Core::AboutWindow::Draw()
{
	ImGui::ShowAboutWindow(&IsOpen);
}

Sphynx::Core::DebugWindow::DebugWindow()
{
	App = GetApplication();
	eventsystem = App->GetAppEventSystem();
	window = App->GetMainWindow();
	memset(TitleBuffer, 0, sizeof(TitleBuffer));
}

//Long Shit for centralized debugging.
void Sphynx::Core::DebugWindow::Draw()
{
	if (ImGui::Begin("Debug Window", &IsOpen)) {
		if (ImGui::CollapsingHeader("ImGui")) {
			ImGui::Indent();
			if (ImGui::Button("Show Demo Window"))
			{
				Imgui::AddOverlayWindow(new Sphynx::Core::DemoWindow());
			}
			ImGui::Text("ImGui Windows:");
			auto ol = Imgui::GetTypeInfoList();
			if (ImGui::BeginListBox("##imguiwindows", ImVec2(FLT_MIN, ImGui::GetTextLineHeightWithSpacing() * 5)))
			{
				static int selected = -1;
				int i = 0;
				for (auto& name : ol) {
					//Text Formating
					std::array<char, 5> index = std::array<char,5>();
					auto [ptr, ec] = std::to_chars(index.data(), index.data() + 5, i);
					char d[256];
					memset(d, '\n', sizeof(char) * (strlen(name) + index.size()));
					strcpy_s(d, name);
					strcat_s(d, " ##");
					strcat_s(d, index.data());
					//End Text Formating
					if (ImGui::Selectable(d, selected == i)) {
						selected = i;
					}
					i++;
				}
				ImGui::EndListBox();
			}
			ImGui::Text("Number Of Window = %i", Imgui::GetNumberOfWindows());
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Window")) {
			ImGui::Indent();
			ImGui::Text("Main Window Width:%i", window->GetWidth());
			ImGui::Text("Main Window Height:%i", window->GetHeight());
			auto fb = Sphynx::Core::SceneManager::GetScene().GetPrimaryCamera()->GetFrameBuffer();
			ImGui::Text("Camera FrameBuffer Width:%i", fb->GetColorAttachment(0)->GetWidth());
			ImGui::Text("Camera FrameBuffer Height:%i", fb->GetColorAttachment(0)->GetHeight());
			if (ImGui::Button("Save Framebuffer")) {
				stbi_flip_vertically_on_write(true);
				fb->Bind();
				stbi_write_bmp("ScreenShot.bmp", fb->GetColorAttachment(0)->GetWidth(), fb->GetColorAttachment(0)->GetHeight(),
					4, fb->GetColorAttachment(0)->ReadAllPixels(Sphynx::Core::Graphics::TextureDataFormat::UInt_8_8_8_8_REV));
				fb->Unbind();
			}
			bool vs = window->IsVsyncEnabled();
			if (ImGui::Checkbox("Vsync", &vs)) {
				window->SetVsync(vs);
			}
			ImGui::InputText("Window Title", TitleBuffer, sizeof(TitleBuffer));
			if (ImGui::Button("Set Title")) {
				window->ChangeTitle(TitleBuffer);
			}
			static int bnds[2];
			ImGui::InputInt2("Window Size", bnds);
			if (ImGui::Button("Set Size")) {
				window->Resize(bnds[0], bnds[1]);
			}
			ImGui::Unindent();
		}
		if (ImGui::CollapsingHeader("Time")) {
			ImGui::Indent();
			ImGui::Text("FPS:%i", (int)(1 / Time::GetDeltaTime()));
			ImGui::Text("Delta Time:%f", Time::GetDeltaTime());
			ImGui::Unindent();
		}
		ImGui::End();
	}
}