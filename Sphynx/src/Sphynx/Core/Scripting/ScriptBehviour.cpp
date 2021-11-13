#include "pch.h"
#include "ScriptBehviour.h"

Sphynx::Core::Scripting::ScriptBehviour::ScriptBehviour(const char* name, GameObject* holder, void* scriptObject, asITypeInfo* typeinfo)
	: Name(name), Holder(holder), ScriptObject(scriptObject), TypeInfo(typeinfo)
{
}
