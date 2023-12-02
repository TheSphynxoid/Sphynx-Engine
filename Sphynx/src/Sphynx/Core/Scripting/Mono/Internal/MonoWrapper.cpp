#include "pch.h"
#include "MonoWrapper.h"
#include "mono/metadata/object.h"
#include "mono/metadata/class.h"
#include "mono/metadata/appdomain.h"

using namespace Sphynx::Mono;

Sphynx::Mono::Class::Class(MonoClass* monoclass)
{
	WrappedClass = monoclass;
}

Object Sphynx::Mono::Class::InstantiateDefault()
{
	return Object(this);
}

Sphynx::Mono::Object Sphynx::Mono::Class::Instantiate(void** Params, size_t count)
{
	return Object(this);
}

const char* Sphynx::Mono::Class::GetName()
{
	return mono_class_get_name(WrappedClass);
}

void* Sphynx::Mono::Class::GetNative()
{
	return WrappedClass;
}

Sphynx::Mono::Object::Object(Class* c)
{
	InstanceOf = c;
	WrappedObject = mono_object_new(mono_domain_get(), (MonoClass*)c->GetNative());
	mono_runtime_object_init(WrappedObject);
}

Sphynx::Mono::Object::Object(Class* c, MonoObject* obj)
{
	InstanceOf = c;
	WrappedObject = obj;
}
