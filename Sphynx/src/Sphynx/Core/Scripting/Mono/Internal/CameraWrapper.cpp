#include "pch.h"
#include "CameraWrapper.h"
#include "../MonoRuntime.h"

static MonoClass* CameraClass;
static MonoClassField* PrimCam;
static MonoClassField* CamID;
static MonoObject* ManagedPrimCam;
static Sphynx::Mono::GameObjectWrapper CamGO;

void Sphynx::Mono::CameraWrapper::Setup()
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<Events::OnSceneStart>(&SetPrimaryCamera);
	CameraClass = mono_class_from_name(MonoRuntime::GetCoreAssemblyImage(), "Sphynx", "Camera");
	CamID = mono_class_get_field_from_name(CameraClass, "InstanceID");
	PrimCam = mono_class_get_field_from_name(CameraClass, "PrimCam");
	//WHY THE F*** IS MONO DOCS ALWAYS DOWN
}

void Sphynx::Mono::CameraWrapper::SetPrimaryCamera(Events::OnSceneStart& e)noexcept
{
	CamGO = GameObjectWrapper::CreateGameObject();
	ManagedPrimCam = mono_object_new(MonoRuntime::GetCoreDomain(), CameraClass);
	Camera* Cam = Core::SceneManager::GetCurrentScene().GetPrimaryCamera();
	EntityID camID = (EntityID)Cam->GetID();
	mono_field_set_value(ManagedPrimCam, CamID, &camID);
	mono_field_static_set_value(mono_class_vtable(MonoRuntime::GetCoreDomain(), CameraClass), PrimCam, ManagedPrimCam);
}
