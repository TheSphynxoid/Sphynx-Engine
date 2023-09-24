#include "pch.h"
#ifndef Sphynx_Internal_Mono
#define Sphynx_Internal_Mono
#define MonoExport __declspec(dllexport)
#include "Logger.h"
#include "Component.h"
#include "Core/Factories/ComponentFactory.h"
//#define ExculdeEntryPoint
//#include "../Sphynx.h"
#include "Core/Graphics/Window.h"
#include <glm/glm.hpp>

extern "C" {

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
	//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
}

extern MonoDomain* Appdomain;

namespace Sphynx::Mono::Internal {

	Core::IWindow* MainWindow;

	MonoExport void NativeFinalize(void* CompPtr) {
		auto VComp = (Component*)CompPtr;
		Sphynx::Core::Internal::ComponentFactory::RemoveComponent(VComp->GetGameObject(), VComp);
	}

	MonoExport void spdLog(MonoString* string, spdlog::level::level_enum level) {
		Logger::GetInternalLogger()->log(level, mono_string_to_utf8(string));
	}

	template<class T>
	MonoExport float Distance(T left, T right) {
		return glm::distance(left, right);
	}
	template<class T>
	MonoExport float Dot(T left, T right) {
		return glm::dot(left, right);
	}
	MonoExport glm::vec3 Cross(glm::vec3 left, glm::vec3 right) {
		return glm::cross(left, right);
	}

	MonoExport void SetTitle(MonoString* string) {
		MainWindow->ChangeTitle(mono_string_to_utf8(string));
	}
	MonoExport MonoString* GetTitle() {
		return mono_string_new(Appdomain, MainWindow->GetTitle());
	}
	MonoExport void SetVsync(bool vsync) {
		MainWindow->SetVsync(vsync);
	}
	MonoExport bool GetVsync() {
		return MainWindow->IsVsyncEnabled();
	}
	MonoExport void SetSize(int width,int height) {
		MainWindow->Resize(width, height);
	}
	MonoExport Sphynx::Core::Bounds GetSize() {
		return MainWindow->GetBounds();
	}
	void RegisterInternalCalls() {
		MainWindow = GetApplication()->GetMainWindow();

		//Sphynx.Core.Native.NativeComponent
		mono_add_internal_call("Sphynx.Core.Native.NativeComponent::NativeFinalize", &NativeFinalize);
		//Sphynx.Logger
		mono_add_internal_call("Sphynx.Logger::spdLog", &spdLog);
		//Sphynx.Vector2
		mono_add_internal_call("Sphynx.Vector2::Distance", &Distance<glm::vec2>);
		mono_add_internal_call("Sphynx.Vector2::Dot", &Dot<glm::vec2>);
		//Sphynx.Vector3
		mono_add_internal_call("Sphynx.Vector3::Distance", &Distance<glm::vec3>);
		mono_add_internal_call("Sphynx.Vector3::Dot", &Dot<glm::vec3>);
		mono_add_internal_call("Sphynx.Vector3::Cross", &Cross);
		//Sphynx.Vector4
		mono_add_internal_call("Sphynx.Vector2::Distance", &Distance<glm::vec4>);
		mono_add_internal_call("Sphynx.Vector2::Dot", &Dot<glm::vec4>);
		//Sphynx.Core.Graphics.Window
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetTitle", &SetTitle);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetTitle", &GetTitle);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetVsync", &SetVsync);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetVsync", &GetVsync);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetSize", &SetSize);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetSize", &GetSize);
	}
}
#endif