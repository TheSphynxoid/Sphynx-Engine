#include "pch.h"
#ifndef Sphynx_Internal_Mono
#define Sphynx_Internal_Mono
#ifdef Platform_Windows
#define MonoExport /*__declspec(dllexport)*/
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
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/Graphics/Pipeline/Buffer.h"
extern "C" {
#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
	//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
}
#include "Internal/GameObjectWrapper.h"
#include "ResourceManager.h"

extern MonoDomain* Appdomain;

using namespace Sphynx::Core::Graphics;

namespace Sphynx::Mono::Internal {

	Core::IWindow* MainWindow;

	MonoExport void NativeFinalize(void* CompPtr) {
		auto VComp = (Component*)CompPtr;
		Sphynx::Core::Internal::ComponentFactory::RemoveComponent(VComp->GetGameObject(), VComp);
	}

	MonoExport void spdLog(MonoString* string, spdlog::level::level_enum level) {
		Logger::GetInternalLogger()->flush();
		auto msg = mono_string_to_utf8(string);
		Logger::GetInternalLogger()->log(level, msg);
		mono_free(msg);
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
		auto title = mono_string_to_utf8(string);
		MainWindow->ChangeTitle(title);
		mono_free(title);
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
	MonoExport void SetSize(int width, int height) {
		MainWindow->Resize(width, height);
	}
	MonoExport Sphynx::Core::Bounds GetSize() {
		return MainWindow->GetBounds();
	}
	MonoExport int GetKeyState(int key) {
		return (int)Input::GetKeyState((Keys)key).action;
	}
	MonoExport int GetButtonState(int button) {
		return (int)Input::GetMouseButtonState((MouseButton)button).action;
	}
	MonoExport glm::vec2 GetMousePosition() {
		return Input::GetMousePosition();
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
	struct TexInfo {
		void* NativeID;
		TextureDataFormat dataFormat;
		TextureFormat format;
		glm::vec3 Dimension;
	};
	MonoExport TexInfo GetTexInfo(Texture* tex)noexcept {
		TexInfo info;
		info.NativeID = tex->GetNativeID();
		info.dataFormat = tex->GetDataFormat();
		info.format = tex->GetFormat();
		info.Dimension = { tex->GetWidth(),tex->GetHeight(),tex->GetDepth() };
		return info;
	}
	MonoExport Shader* ShaderLoader(MonoString* path, Sphynx::Core::Graphics::ShaderType type) {
		auto p = mono_string_to_utf8(path);
		auto shader = Sphynx::ResourceManager::LoadShader(p, type);
		mono_free(p);
		return shader;
	}
	MonoExport Texture* TextureLoader(MonoString* path, TextureType type, bool compress) {
		auto p = mono_string_to_utf8(path);
		auto tex = ResourceManager::LoadTexture(p, type, compress);
		mono_free(p);
		return tex;
	}
	MonoExport void MatBind(Material* mat)noexcept {
		mat->Bind();
	}
	MonoExport void MatUnbind(Material* mat)noexcept {
		mat->Unbind();
	}
	MonoExport void AddTex(Material* mat, Texture* tex) {
		mat->AddTexture(tex);
	}	
	MonoExport void RemoveTex(Material* mat, Texture* tex) {
		mat->RemoveTexture(tex);
	}	
	MonoExport void SetTex(Material* mat, Texture* tex, unsigned int index) {
		mat->SetTexture(tex, index);
	}
	MonoExport int GetUniLoc(Material* mat, MonoString* name) {
		auto uniname = mono_string_to_utf8(name);
		auto loc = mat->GetUniformLocation(uniname);
		mono_free(uniname);
		return loc;
	}	
	MonoExport void SetUni(Material* mat, Uniform* uni, int loc) {
		mat->SetUniform(uni, loc);
	}
	MonoExport Uniform* CreateUniform(void* data, ShaderDataType type) {
		return Uniform::Create(data, type);
	}
	MonoExport void ReallocGPUBuf(GPUBuffer* buf,size_t size,void* data) {
		buf->Reallocate(size, data);
	}
	MonoExport void InvalidateGPUBuf(GPUBuffer* buf) {
		buf->Invalidate();
	}
	MonoExport void SetDataGPUBuf(GPUBuffer* buf, void* data, size_t size, size_t offset) {
		buf->SetData(data, size, offset);
	}
	MonoExport void BufBind(GPUBuffer* buf) {
		buf->Bind();
	}
	MonoExport void BufUnbind(GPUBuffer* buf) {
		buf->Unbind();
	}
	MonoExport void Submit_Internal(RenderObject ro) {
		GetMainWindow()->GetRenderer()->Submit(ro);
	}
	MonoExport void Render() {
		GetMainWindow()->GetRenderer()->Render();
	}	
	MonoExport void Clear() {
		GetMainWindow()->GetRenderer()->Clear();
	}
	MonoExport void SetViewport(Viewport v) {
		GetMainWindow()->GetRenderer()->SetViewport(v);
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
		mono_add_internal_call("Sphynx.Vector4::Distance", (void*)&Distance<glm::vec4>);
		mono_add_internal_call("Sphynx.Vector4::Dot", (void*)&Dot<glm::vec4>);
		//Sphynx.Input
		mono_add_internal_call("Sphynx.Input::InternalKeyState", (void*)&GetKeyState);
		mono_add_internal_call("Sphynx.Input::InternalButtonState", (void*)&GetButtonState);
		mono_add_internal_call("Sphynx.Input::GetMousePosition", (void*)&GetMousePosition);
		//Sphynx.Core.Graphics.Window
		mono_add_internal_call("Sphynx.Graphics.Window::SetTitle", (void*)&SetTitle);
		mono_add_internal_call("Sphynx.Graphics.Window::GetTitle", (void*)&GetTitle);
		mono_add_internal_call("Sphynx.Graphics.Window::SetVsync", (void*)&SetVsync);
		mono_add_internal_call("Sphynx.Graphics.Window::GetVsync", (void*)&GetVsync);
		mono_add_internal_call("Sphynx.Graphics.Window::SetSize", (void*)&SetSize);
		mono_add_internal_call("Sphynx.Graphics.Window::GetSize", (void*)&GetSize);
		//Sphynx.Transform
		mono_add_internal_call("Sphynx.Transform::SetPosition", (void*)&SetPosition);
		mono_add_internal_call("Sphynx.Transform::GetPosition", (void*)&GetPosition);
		//Sphynx.Core.AssetManager
		mono_add_internal_call("Sphynx.Core.AssetManager::RM_LoadShader", (void*)&ShaderLoader);
		mono_add_internal_call("Sphynx.Core.AssetManager::RM_LoadTexture", (void*)&TextureLoader);
		//Sphynx.Graphics.Texture
		mono_add_internal_call("Sphynx.Graphics.Texture::CreateTexture", 
			static_cast<Texture* (*)(void*, int, int, int, TextureType, int, TextureFormat, TextureDataFormat, TextureWrappingMode, 
				TextureFilterMode, TextureMipmapMode)>(&Texture::Create));
		mono_add_internal_call("Sphynx.Graphics.Texture::GetTexInfo", (void*)&GetTexInfo);
		mono_add_internal_call("Sphynx.Graphics.Texture::GetDefaultFilterMode", &Texture::GetDefaultFilterMode);
		mono_add_internal_call("Sphynx.Graphics.Texture::GetDefaultWrappingMode", &Texture::GetDefaultWrappingMode);
		mono_add_internal_call("Sphynx.Graphics.Texture::GetDefaultMipmapMode", &Texture::GetDefaultMipmapMode);
		//Sphynx.Graphics.Material
		mono_add_internal_call("Sphynx.Graphics.Material::CreateMaterial(Sphynx.Graphics.Material/NativeShaderPack)",
			static_cast<Material* (*)(const ShaderPack&)>(&Material::Create));
		mono_add_internal_call("Sphynx.Graphics.Material::"
			"CreateMaterial",
			static_cast<Material* (*)(const ShaderPack&, std::initializer_list<Texture*>)>(&Material::Create));
		mono_add_internal_call("Sphynx.Graphics.Material::MatBind", &MatBind);
		mono_add_internal_call("Sphynx.Graphics.Material::MatUnbind", &MatUnbind);
		mono_add_internal_call("Sphynx.Graphics.Material::MatGetDefaultShader", &Material::GetDefaultShader);
		mono_add_internal_call("Sphynx.Graphics.Material::AddTex", &AddTex);
		mono_add_internal_call("Sphynx.Graphics.Material::RemoveTex", &RemoveTex);
		mono_add_internal_call("Sphynx.Graphics.Material::SetTex", &SetTex);
		mono_add_internal_call("Sphynx.Graphics.Material::GetUniformLoc", &GetUniLoc);
		mono_add_internal_call("Sphynx.Graphics.Material::SetUni", &SetUni);
		//Sphynx.Graphics.Uniform
		mono_add_internal_call("Sphynx.Graphics.Uniform::CreateUniform", &CreateUniform);
		//Sphynx.Graphics.Utils
		mono_add_internal_call("Sphynx.Graphics.Utils::GetDataTypeSize_Internal", &GetShaderDataTypeSize);
		//Sphynx.Graphics.GPUBuffer
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::CreateBuffer", &GPUBuffer::Create);
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::ReallocateBuffer", &ReallocGPUBuf);
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::InvalidateBuffer", &InvalidateGPUBuf);
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::SetBufferData", &SetDataGPUBuf);
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::BindBuffer", &BufBind);
		mono_add_internal_call("Sphynx.Graphics.GPUBuffer::UnbindBuffer", &BufUnbind);
		//Sphynx.Graphics.Renderer
		mono_add_internal_call("Sphynx.Graphics.Renderer::Submit_Internal", &Submit_Internal);
		mono_add_internal_call("Sphynx.Graphics.Renderer::Render", &Render);
		mono_add_internal_call("Sphynx.Graphics.Renderer::Clear", &Clear);
		mono_add_internal_call("Sphynx.Graphics.Renderer::SetViewport", &SetViewport);
	}
}
#endif