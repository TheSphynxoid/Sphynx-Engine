#include "pch.h"
#include "MonoRuntime.h"


extern "C" {

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
}

//Seperation of Code.
#include "MonoInternalCalls.cpp"

static MonoDomain* JITdomain;
static MonoDomain* Appdomain;
static MonoAssembly* ScriptAssembly = nullptr;
static MonoAssembly* GameAssembly = nullptr;
static MonoImage* ScriptImage = nullptr;
static MonoImage* GameImage = nullptr;


namespace Sphynx::Mono::Internal {
	
	std::pair<MonoAssembly*, MonoImage*> LoadAssembly(std::string AssemblyPath) 
	{
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_full(AssemblyPath.c_str(), &status, false);

		if (status != MonoImageOpenStatus::MONO_IMAGE_OK) {
			Core_Error(mono_image_strerror(status));
			return { nullptr,nullptr };
		}
		ScriptImage == nullptr;

		MonoAssembly* assembly = mono_assembly_load_from_full(image, AssemblyPath.c_str(), &status, 0);
		
		return { assembly, image };
	}

}

Sphynx::Mono::MonoRuntime::MonoRuntime() : isAlive(true)
{
	mono_set_assemblies_path("data");

	JITdomain = mono_jit_init("ScriptJIT");
}

Sphynx::Mono::MonoRuntime::~MonoRuntime()
{
	if (isAlive) {
		Shutdown();
	}
}

Sphynx::Mono::MonoRuntime::MonoRuntime(std::string AssemblyPath) : MonoRuntime()
{

	Appdomain = mono_domain_create_appdomain("GameDomain", nullptr);

	//Load the ScriptAssembly
	std::tie(ScriptAssembly, ScriptImage) = Internal::LoadAssembly("ScriptAssembly.dll");

	//Load the AppAssembly
	std::tie(GameAssembly,GameImage) = Internal::LoadAssembly(AssemblyPath.c_str());

	static bool HasRegisteredInternals = false;

	if (!HasRegisteredInternals) {
		Internal::RegisterInternalCalls();
	}

}

void Sphynx::Mono::MonoRuntime::Start()
{

}

void Sphynx::Mono::MonoRuntime::Update()
{
}

void Sphynx::Mono::MonoRuntime::Shutdown()
{
	mono_domain_unload(Appdomain);
	mono_domain_finalize(Appdomain, 2000);
	mono_domain_free(Appdomain, true);
	mono_jit_cleanup(JITdomain);
	isAlive = false;
}
