#include "pch.h"
#include "ScriptBehviour.h"
#include "angelscript.h"

Sphynx::Core::Scripting::ScriptBehaviour::ScriptBehaviour(GameObject* holder, asIScriptObject* scriptObject, asITypeInfo* typeinfo)
	: Name(typeinfo->GetName()), Holder(holder), ScriptObject(scriptObject), TypeInfo(typeinfo)
{
}
