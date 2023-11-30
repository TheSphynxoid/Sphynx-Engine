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
	return Object(this, mono_object_new(mono_domain_get(), WrappedClass));
}

Sphynx::Mono::Object Sphynx::Mono::Class::Instantiate()
{
	return Object();
}

Sphynx::Mono::Object::Object(Class* c, MonoObject* obj)
{
	InstanceOf = c;
	WrappedObject = obj;
}
