#include "pch.h"
#ifndef Sphynx_Internal_Mono
#define Sphynx_Internal_Mono
#define MonoExport __declspec(dllexport)
#include "Logger.h"

extern "C" {

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
	//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
}

namespace Sphynx::Mono::Internal {

	MonoExport void NativeFinalize(MonoObject* Native) {
		
	}

	MonoExport void spdLog(MonoString* string, spdlog::level::level_enum level) {
		Logger::GetClientLogger()->log(level, mono_string_to_utf8(string));
	}

	void RegisterInternalCalls() {
		//Sphynx.Core.Native.NativeComponent
		mono_add_internal_call("Sphynx.Core.Native.NativeComponent::NativeFinalize", &NativeFinalize);
		//Sphynx.Logger
		mono_add_internal_call("Sphynx.Logger::spdLog", &spdLog);
	}
}
#endif