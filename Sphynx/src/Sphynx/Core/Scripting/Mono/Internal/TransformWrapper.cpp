#include "pch.h"
#include "TransformWrapper.h"
#include "../MonoRuntime.h"

Sphynx::Mono::TransformWrapper::TransformWrapper()
{
}

Sphynx::Mono::TransformWrapper::TransformWrapper(MonoObject* GO)
{
	TransformObject = MonoRuntime::CreateInitializedObject(TransformClass);
}
