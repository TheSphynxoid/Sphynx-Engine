#include "pch.h"
#include "GameObjectWrapper.h"
#include "../MonoRuntime.h"

Sphynx::Mono::GameObjectWrapper::GameObjectWrapper(GameObject* go)
{
	gameObject = go;
	auto GoClass = MonoRuntime::GetCommonType("Sphynx.GameObject");
	Managedobj = MonoRuntime::CreateObject(GoClass);
}
