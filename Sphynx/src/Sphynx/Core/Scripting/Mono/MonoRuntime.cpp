#include "pch.h"
#include "MonoRuntime.h"


extern "C" {

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"
//#include "mono/metadata/tabledefs.h"
#include "mono/metadata/mono-debug.h"
#include "mono/metadata/threads.h"
<<<<<<< HEAD
#include "mono/metadata/mono-debug.h"
=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
}

//Seperation of Code.
#include "MonoInternalCalls.cpp"

static MonoDomain* JITdomain;
static MonoDomain* Appdomain;
static MonoAssembly* ScriptAssembly = nullptr;
static MonoAssembly* GameAssembly = nullptr;
static MonoImage* ScriptImage = nullptr;
static MonoImage* GameImage = nullptr;

<<<<<<< HEAD
//Sphynx Engine Internals
void (_stdcall *WinResizeThunk)(int, int, MonoException**) = nullptr;

const std::unordered_map<std::string, MonoClass*> CommunTypes = {
	{"System.Object",mono_get_object_class()},
	{"System.Int16",mono_get_int16_class()},
	{"System.Int32",mono_get_int32_class()},
	{"System.Int64",mono_get_int64_class()},
	{"System.UInt16",mono_get_uint16_class()},
	{"System.UInt32",mono_get_uint32_class()},
	{"System.UInt64",mono_get_uint64_class()},
	{"System.Double",mono_get_double_class()},
	{"System.Single",mono_get_single_class()},
	{"System.IntPtr",mono_get_intptr_class()},
	{"System.UIntPtr",mono_get_uintptr_class()},
	{"System.String",mono_get_string_class()},
	{"System.Boolean",mono_get_boolean_class()},
	{"System.Byte",mono_get_byte_class()},
	{"System.SByte",mono_get_sbyte_class()},
	{"System.Char",mono_get_char_class()},
	{"System.Threading.Thread",mono_get_thread_class()},
	{"System.Enum",mono_get_enum_class()},
	{"System.Exception",mono_get_exception_class()},
	{"void",mono_get_void_class()}
};

=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d

namespace Sphynx::Mono::Internal {
	
	std::pair<MonoAssembly*, MonoImage*> LoadAssembly(std::string AssemblyPath) 
	{
		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_full(AssemblyPath.c_str(), &status, false);

		if (status != MonoImageOpenStatus::MONO_IMAGE_OK) {
			Core_Error(mono_image_strerror(status));
			return { nullptr,nullptr };
		}
<<<<<<< HEAD
=======
		ScriptImage == nullptr;
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d

		MonoAssembly* assembly = mono_assembly_load_from_full(image, AssemblyPath.c_str(), &status, 0);
		
		return { assembly, image };
	}

}

Sphynx::Mono::MonoRuntime::MonoRuntime() : isAlive(true)
{
	mono_set_assemblies_path("data");

<<<<<<< HEAD
#ifdef DEBUG
	const char* argv[2] = {
	"--debugger-agent=transport=dt_socket,address=127.0.0.1:2550,server=y,suspend=n,loglevel=3,logfile=MonoDebugger.log",
	"--soft-breakpoints"
	};

	mono_jit_parse_options(2, (char**)argv);
	mono_debug_init(MONO_DEBUG_FORMAT_MONO);
#endif

=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
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
<<<<<<< HEAD
	Appdomain = mono_domain_create_appdomain("GameDomain", nullptr);
	mono_thread_attach(Appdomain);
=======

	Appdomain = mono_domain_create_appdomain("GameDomain", nullptr);
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d

	//Load the ScriptAssembly
	std::tie(ScriptAssembly, ScriptImage) = Internal::LoadAssembly("ScriptAssembly.dll");

	//Load the AppAssembly
	std::tie(GameAssembly,GameImage) = Internal::LoadAssembly(AssemblyPath.c_str());

<<<<<<< HEAD
	MonoClass* CompClass = mono_class_from_name(ScriptImage, "Sphynx", "Component");

=======
>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
	static bool HasRegisteredInternals = false;

	if (!HasRegisteredInternals) {
		Internal::RegisterInternalCalls();
<<<<<<< HEAD
		auto WinResize = mono_class_get_method_from_name(mono_class_from_name(ScriptImage, "Sphynx.Core.Graphics", "Window"), "InvokeResize", 2);
		WinResizeThunk = (void(_stdcall *)(int, int,MonoException**))(mono_method_get_unmanaged_thunk(WinResize));

		static auto ResizeLambda = [](Events::OnWindowResize& e)
			{
				MonoException* ex = nullptr;
				(*WinResizeThunk)(e.Width, e.Height, &ex);
				if (ex) { 
					mono_print_unhandled_exception((MonoObject*)ex);
					throw std::exception(); 
				}
			};
		Sphynx::Events::GlobalEventSystem::GetInstance()->Subscribe<Events::OnWindowResize>(ResizeLambda);
		HasRegisteredInternals = true;
	}

	//Get All classes that inherit component from GameAssembly.
	//See http://docs.go-mono.com/?link=xhtml%3adeploy%2fmono-api-metadata.html
	const MonoTableInfo* TypeDefTable = mono_image_get_table_info(GameImage, MONO_TABLE_TYPEDEF);
	/*
	TYPEDEF table contains :
		MONO_TYPEDEF_FLAGS
		MONO_TYPEDEF_NAME
		MONO_TYPEDEF_NAMESPACE
		MONO_TYPEDEF_EXTENDS
		MONO_TYPEDEF_FIELD_LIST
		MONO_TYPEDEF_METHOD_LIST 
	*/
	int TypeCount = mono_table_info_get_rows(TypeDefTable);
	//Loop over Types in the table.
	for (int i = 0; i < TypeCount; i++) {
		uint32_t cols[MONO_TYPEDEF_SIZE];
		mono_metadata_decode_row(TypeDefTable, i, cols, MONO_TYPEDEF_SIZE);

		//Get The Name and namespace of the classes
		const char* Namespace = mono_metadata_string_heap(GameImage, cols[MONO_TYPEDEF_NAMESPACE]);
		const char* Classname = mono_metadata_string_heap(GameImage, cols[MONO_TYPEDEF_NAME]);
		//Size of the Fullname of class (Namespace + "." + Classname)
		const size_t NamespaceLen = strlen(Namespace);
		const size_t ClassnameLen = strlen(Classname);
		//Creating the string
		std::string Fullname = std::string(NamespaceLen + ClassnameLen + 1, ' ');
		//Formatting
		strcpy(&Fullname[0], Namespace);
		Fullname[NamespaceLen] = '.';
		strcpy(&Fullname[NamespaceLen + 1], Classname);
		
		Core_Info(Fullname);

		MonoClass* monoClass = mono_class_from_name(GameImage, Namespace, Classname);
		bool IsComponent = mono_class_is_subclass_of(monoClass, CompClass, true);

	}
=======
	}

>>>>>>> 8e4e7476835c79b1abd56bf61659663c37a76c4d
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
