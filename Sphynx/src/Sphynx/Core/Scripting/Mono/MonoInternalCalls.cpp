#include "pch.h"
#include "MonoInternalCalls.h"
#ifndef Sphynx_Internal_Mono
#define Sphynx_Internal_Mono
#ifdef Platform_Windows
#define MonoExport /*__declspec(dllexport)*/
#elif
#define MonoExport
#endif
#include "Core/SceneManager.h"
#include "Scene.h"
#include "Logger.h"
#include "Component.h"
#include "Camera.h"
#include "Core/Factories/ComponentFactory.h"
#include "Core/Graphics/Window.h"
#include <glm/glm.hpp>
#include "Internal/NativeComponent.h"
#include "Core/Graphics/Pipeline/Renderer.h"
#include "Core/Graphics/Pipeline/Material.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Core/Graphics/Pipeline/Mesh.h"
#include "Core/Graphics/Pipeline/Buffer.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
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

//Links with Appdomain defined in MonoRuntime.cpp.
extern MonoDomain* Appdomain;

using namespace Sphynx::Core::Graphics;

namespace Sphynx::Mono::Internal {

	Core::IWindow* MainWindow;

	template<typename T>
	std::vector<T> make_vector(T* t, size_t count)
	{
		auto vec = std::vector<T>(count);
		vec.assign(t, &t[count - 1]);
		return vec;
	}

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
	MonoExport struct TexInfo {
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
	MonoExport void BindTex(Texture* tex) {
		tex->Bind();
	}
	MonoExport void UnbindTex(Texture* tex) {
		tex->Unbind();
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
		const auto loc = mat->GetUniformLocation(uniname);
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
	MonoExport void* malloc(size_t size) {
		//For now, we return the pointer, in future maybe we'll manage buffer given to C#.
		return ::malloc(size);
	}
	MonoExport void* realloc(void* p, size_t size) {
		return ::realloc(p, size);
	}
	MonoExport void free(void* p) {
		::free(p);
	}
	MonoExport void memcpy(MonoArray* array, size_t offset, size_t count, void* nativePtr) {
		char* data = mono_array_addr_with_size(array, sizeof(char), offset);
		memcpy_s(nativePtr, sizeof(char) * count, data, sizeof(char) * count);
	}
	MonoExport void memset(void* ptr, size_t size, int val) {
		::memset(ptr, val, size);
	}
	MonoExport void* ArrayToPointer(MonoArray* Array, int IndexFirst) {
		auto elemsize = mono_array_element_size(mono_object_get_class((MonoObject*)Array));
		char* data = mono_array_addr_with_size(Array,  elemsize, IndexFirst);
		return data;
	}
	struct VBLayout {
		BufferElement* elems;
		int count;
		unsigned int stride;
	};
	MonoExport void SetVBLayout(VertexBuffer* vb, VBLayout layout) {
		vb->SetDataLayout(BufferLayout(std::initializer_list<BufferElement>(layout.elems, &layout.elems[layout.count - 1])));
	}
	MonoExport size_t GetVBSize(VertexBuffer* vb) {
		return vb->GetSize();
	}
	MonoExport GPUBuffer* VBToGPUBuffer(VertexBuffer* vb) {
		return vb;
	}	
	MonoExport GPUBuffer* IBToGPUBuffer(IndexBuffer* vb) {
		return vb;
	}
	MonoExport Sphynx::Core::Graphics::Mesh* MeshCreateVBs(VertexBuffer** vbs, int Count, IndexBuffer* ib) {
		return Mesh::Create(make_vector(vbs, Count), ib);
	}
	MonoExport void AddVB(Mesh* mesh, VertexBuffer* vb) {
		mesh->AddVertexBuffer(vb);
	}
	MonoExport void SetVBs(Mesh* mesh, int Count, VertexBuffer** vbs) {
		mesh->SetVertexBuffers(make_vector(vbs, Count));
	}
	MonoExport void SetIB(Mesh* mesh, IndexBuffer* ib) {
		mesh->SetIndexBuffer(ib);
	}
	MonoExport void MeshBind(Mesh* mesh) {
		mesh->Bind();
	}
	MonoExport void MeshUnbind(Mesh* mesh) {
		mesh->Unbind();
	}
	MonoExport void MeshSetMode(Mesh* mesh, RenderMode mode) {
		mesh->SetRenderMode(mode);
	}
	MonoExport Camera* GetPrimaryCamera() {
		return Sphynx::Core::SceneManager::GetScene().GetPrimaryCamera();
	}
	MonoExport Component* GetNativeCompByName(GameObject* go, MonoString* str) {
		const auto name = mono_string_to_utf8(str);
		for (const auto& comp : go->GetComponents()) {
			if (!strcmp(comp->GetName(), name)) {
				return comp;
			}
		}
		mono_free(name);
	}
	MonoExport void CoreCheck(void* anyObj, const bool unbox) {
		if (unbox) {
			anyObj = mono_object_unbox((MonoObject*)anyObj);
		}
		__debugbreak();
	}
	MonoExport FrameBuffer* CreateFB(int width, int height, Texture** texArray, int count)
	{
		return FrameBuffer::Create(width, height, std::initializer_list(texArray, &texArray[count - 1]));
	}
	MonoExport void ReleaseFB(FrameBuffer* fb)
	{
		delete fb;
	}
	MonoExport FrameBuffer* GetDefaultFB()
	{
		return FrameBuffer::GetDefaultFrameBuffer();
	}

	void RegisterInternalCalls() {
		MainWindow = GetApplication()->GetMainWindow();
		//Sphynx.Core.Native.ComponentFactory
		mono_add_internal_call("Sphynx.Core.Native.ComponentFactory::CreateNative", (void*)&CreateNativeComponent);
		//Sphynx.Core.Native.NativeComponent
		mono_add_internal_call("Sphynx.Core.Native.NativeComponent::NativeFinalize", (void*)&NativeFinalize);
		//Sphynx.Core.Native.NativeComponentFactory
		mono_add_internal_call("Sphynx.Core.Native.NativeComponentFactory::GetNativeCompByName", &GetNativeCompByName);
		//Sphynx.Core.Engine
		mono_add_internal_call("Sphynx.Core.Engine::ArrayToPointer", (void*)&ArrayToPointer);
		mono_add_internal_call("Sphynx.Core.Engine::CoreCheckObject", &CoreCheck);
		//Sphynx.Core.Native.Memory
		mono_add_internal_call("Sphynx.Core.Native.Memory::malloc", &Mono::Internal::malloc);
		mono_add_internal_call("Sphynx.Core.Native.Memory::realloc", &Mono::Internal::realloc);
		mono_add_internal_call("Sphynx.Core.Native.Memory::free", &Mono::Internal::free);
		mono_add_internal_call("Sphynx.Core.Native.Memory::memcpy", &Mono::Internal::memcpy);
		mono_add_internal_call("Sphynx.Core.Native.Memory::memset", &Mono::Internal::memset);
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
		mono_add_internal_call("Sphynx.Graphics.Texture::Bind", &BindTex);
		mono_add_internal_call("Sphynx.Graphics.Texture::Unbind", &UnbindTex);
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
		//Sphynx.Graphics.VertexBuffer
		mono_add_internal_call("Sphynx.Graphics.VertexBuffer::CreateVB", &VertexBuffer::Create);
		mono_add_internal_call("Sphynx.Graphics.VertexBuffer::GetUnderlyingBuffer", &VBToGPUBuffer);
		mono_add_internal_call("Sphynx.Graphics.VertexBuffer::SetLayout", &SetVBLayout);
		mono_add_internal_call("Sphynx.Graphics.VertexBuffer::SetData", &SetDataGPUBuf);
		mono_add_internal_call("Sphynx.Graphics.VertexBuffer::GetVBSize", &GetVBSize);
		//Sphynx.Graphics.IndexBuffer
		mono_add_internal_call("Sphynx.Graphics.IndexBuffer::CreateIB", &IndexBuffer::Create);
		mono_add_internal_call("Sphynx.Graphics.IndexBuffer::GetUnderlyingBuffer", &IBToGPUBuffer);
		mono_add_internal_call("Sphynx.Graphics.IndexBuffer::SetData_int", &SetDataGPUBuf);
		//Sphynx.Graphics.Mesh
		mono_add_internal_call("Sphynx.Graphics.Mesh::CreateEmpty", &Mesh::CreateEmpty);
		mono_add_internal_call("Sphynx.Graphics.Mesh::Create", static_cast<Mesh*(*)(VertexBuffer*, IndexBuffer*)>(&Mesh::Create));
		mono_add_internal_call("Sphynx.Graphics.Mesh::CreateList", &MeshCreateVBs);
		mono_add_internal_call("Sphynx.Graphics.Mesh::AddVB", &AddVB);
		mono_add_internal_call("Sphynx.Graphics.Mesh::SetVBs", &SetVBs);
		mono_add_internal_call("Sphynx.Graphics.Mesh::SetIB", &SetIB);
		mono_add_internal_call("Sphynx.Graphics.Mesh::Bind", &MeshBind);
		mono_add_internal_call("Sphynx.Graphics.Mesh::Unbind", &MeshUnbind);
		mono_add_internal_call("Sphynx.Graphics.Mesh::SetRenderMode", &MeshSetMode);
		//Sphynx.Graphics.FrameBuffer
		mono_add_internal_call("Sphynx.Graphics.FrameBuffer::Create", &CreateFB);
		mono_add_internal_call("Sphynx.Graphics.FrameBuffer::BindDefault", &FrameBuffer::BindDefault);

	}
}
#endif