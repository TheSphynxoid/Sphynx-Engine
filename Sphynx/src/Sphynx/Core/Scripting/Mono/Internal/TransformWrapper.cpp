#include "pch.h"
#include "TransformWrapper.h"
#include "../MonoRuntime.h"

Sphynx::Mono::TransformWrapper::TransformWrapper()
{
	TransformObject = MonoRuntime::CreateObject(TransformClass);
	
}
