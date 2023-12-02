#include "pch.h"
#ifndef Sphynx_Internal_Mono
#define Sphynx_Internal_Mono
#ifdef Platform_Windows
#define MonoExport __declspec(dllexport)
#elif
#define MonoExport
#endif
#include "Logger.h"
#include "Component.h"
#include "Core/Factories/ComponentFactory.h"
//#define ExculdeEntryPoint
//#include "../Sphynx.h"
#include "Core/Graphics/Window.h"
#include <glm/glm.hpp>
#include "Internal/NativeComponent.h"
extern "C" {

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
	//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
}
#include "Internal/GameObjectWrapper.h"

extern MonoDomain* Appdomain;

namespace Sphynx::Mono::Internal {

	Core::IWindow* MainWindow;

	MonoExport void NativeFinalize(void* CompPtr) {
		auto VComp = (Component*)CompPtr;
		Sphynx::Core::Internal::ComponentFactory::RemoveComponent(VComp->GetGameObject(), VComp);
	}

	MonoExport void spdLog(MonoString* string, spdlog::level::level_enum level) {
		Logger::GetInternalLogger()->flush();
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
	MonoExport int GetKeyState(int key) {
		return (int)Input::GetKeyState((Keys)key).action;
	}
	MonoExport Mono::NativeComponent CreateNativeComponent(size_t goID) {
		auto Native = NativeComponent();
		return Native;
	}
	MonoExport void SetPosition(MonoObject* go, glm::vec3 pos) {
		GameObject* gameobj = GameObjectWrapper::GetFromObject_unchecked(go);
		gameobj->GetTransform()->SetPosition(pos);
	}
	MonoExport glm::vec3 GetPosition(MonoObject* go) {
		return GameObjectWrapper::GetFromObject_unchecked(go)->GetTransform()->GetPosition();
	}
	void RegisterInternalCalls() {
		MainWindow = GetApplication()->GetMainWindow();

		mono_add_internal_call("Sphynx.Core.Native.ComponentFactory::CreateNative", (void*)&CreateNativeComponent);
		//Sphynx.Core.Native.NativeComponent
		mono_add_internal_call("Sphynx.Core.Native.NativeComponent::NativeFinalize", (void*)&NativeFinalize);
		//Sphynx.Time
		mono_add_internal_call("Sphynx.Time::GetDeltaTime", (void*)&Time::GetDeltaTime);
		mono_add_internal_call("Sphynx.Time::GetDeltaTicks", (void*)&Time::GetDeltaTicks);
		//Sphynx.Logger
		mono_add_internal_call("Sphynx.Logger::spdLog", (void*)&spdLog);
		//Sphynx.Vector2
		mono_add_internal_call("Sphynx.Vector2::Distance", (void*)&Distance<glm::vec2>);
		mono_add_internal_call("Sphynx.Vector2::Dot", (void*)&Dot<glm::vec2>);
		//Sphynx.Vector3
		mono_add_internal_call("Sphynx.Vector3::Distance", (void*)&Distance<glm::vec3>);
		mono_add_internal_call("Sphynx.Vector3::Dot", (void*)&Dot<glm::vec3>);
		mono_add_internal_call("Sphynx.Vector3::Cross", (void*)&Cross);
		//Sphynx.Vector4
		mono_add_internal_call("Sphynx.Vector2::Distance", (void*)&Distance<glm::vec4>);
		mono_add_internal_call("Sphynx.Vector2::Dot", (void*)&Dot<glm::vec4>);
		//Sphynx.Input
		mono_add_internal_call("Sphynx.Input::InternalKeyState", (void*)&GetKeyState);
		//Sphynx.Core.Graphics.Window
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetTitle", (void*)&SetTitle);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetTitle", (void*)&GetTitle);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetVsync", (void*)&SetVsync);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetVsync", (void*)&GetVsync);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::SetSize", (void*)&SetSize);
		mono_add_internal_call("Sphynx.Core.Graphics.Window::GetSize", (void*)&GetSize);
		//Sphynx.Transform
		mono_add_internal_call("Sphynx.Transform::SetPosition", (void*)&SetPosition);
		mono_add_internal_call("Sphynx.Transform::GetPosition", (void*)&GetPosition);
	}
}
#endif