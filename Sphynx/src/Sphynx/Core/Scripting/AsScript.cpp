#include "pch.h"
#include "AsScript.h"
#include "ScriptingEngine.h"
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Scene.h"
#include "Core/SceneManager.h"
#include <scriptstdstring/scriptstdstring.h>
//Oh No...
#include <scriptstdstring/scriptstdstring.cpp>
#include <scriptbuilder/scriptbuilder.cpp>

using namespace Sphynx;
using namespace Sphynx::Core;
using namespace Sphynx::Core::Graphics;

struct {
	Sphynx::Core::IWindow* IWin;
}Window;

void MessageCallback(const asSMessageInfo* msg, void* param)
{
	const char* type = "ERR ";
	if (msg->type == asMSGTYPE_WARNING)
		Core_Warn("{0} ({1}, {2}) : {3} : {4}", msg->section, msg->row, msg->col, type, msg->message);
	else if (msg->type == asMSGTYPE_INFORMATION)
		Core_Info("{0} ({1}, {2}) : {3} : {4}", msg->section, msg->row, msg->col, type, msg->message);

	//printf("%s (%d, %d) : %s : %s\n", msg->section, msg->row, msg->col, type, msg->message);
}

void Print(std::string& msg) {
	Core_Info(msg);
}

void PrintI(int& msg) {
	Core_Info(msg);
}

void Sphynx::Core::Scripting::AngelScript::OnAppStart(Events::OnApplicationStart& e)
{
	auto r = Engine->RegisterGlobalProperty("Sphynx::Application App", GetApplication());
	r = Engine->RegisterObjectMethod("Sphynx::Application", "bool HasWindow()", asMETHOD(Application, Application::HasWindow), asCALL_THISCALL);
}

void Sphynx::Core::Scripting::AngelScript::OnWindowOpen(Events::OnWindowOpen& e)
{
	auto r = Engine->RegisterGlobalProperty("Sphynx::Window MainWindow", GetMainWindow());
	assert(r >= 0);
}

int getWindowWidth(/*Sphynx::Core::IWindow* win*/) {
	return GetMainWindow()->GetWidth();
}

template<class T>
void ConstructBasic(void* mem) 
{
	new(mem)T();
}

template<class T>
T* CreateBasic() {
	T* t= new T();
	return t;
}

template<class T, typename ...Args>
void ConstuctArgs(void* mem, Args&& ...args) {
	new(mem)T(std::forward<Args>(args));
}

template<class T>
void Destruct(void* mem) {
	((T*)mem)->~T();
}

void ConstructBounds(void* mem, int h, int w) {
	new(mem)(Sphynx::Core::Bounds){ w,h };
}

void ChangeTitle(IWindow* win, std::string& title) {
	win->ChangeTitle(title.c_str());
}

#define SpRegisterEnumValue(Engine,Enum,EnumValue) Engine->RegisterEnumValue(#Enum,#EnumValue,(int)Enum::EnumValue)

template<class T>
class ScriptObjectWrapper {
private:
	int Refs = 0;
	T* Obj;
public:
	ScriptObjectWrapper() {
		Obj = new T();
	}
	void AddRef() { Refs++; };
	void Release() { Refs--; };
};

Sphynx::Core::Scripting::AngelScript::AngelScript()
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<AngelScript, Events::OnApplicationStart>(this, &AngelScript::OnAppStart);
	Events::GlobalEventSystem::GetInstance()->Subscribe<AngelScript, Events::OnWindowOpen>(this, &AngelScript::OnWindowOpen);
	asPrepareMultithread();
	Engine = asCreateScriptEngine();
	Engine->SetMessageCallback(asFunctionPtr(&MessageCallback), NULL, asCALL_CDECL);
	Engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);
	RegisterStdString(Engine);
	Engine->RegisterGlobalFunction("void Print(const string &in)", asFUNCTION(Print), asCALL_CDECL);
	Engine->RegisterGlobalFunction("void PrintI(int &in)", asFUNCTION(PrintI), asCALL_CDECL);
	Engine->SetDefaultNamespace("Sphynx");
	//APPLICATION REGISTRATION
	Engine->RegisterObjectType("Application", 0, asOBJ_REF | asOBJ_NOHANDLE);
	//BOUNDS REGISTRATION
	auto r = Engine->RegisterObjectType("Bounds", sizeof(Bounds),
		asOBJ_VALUE | asOBJ_APP_CLASS_MORE_CONSTRUCTORS | asOBJ_POD | asGetTypeTraits<Bounds>()); assert(r >= 0);
	Engine->RegisterObjectProperty("Bounds", "int Height", asOFFSET(Bounds, Height));
	Engine->RegisterObjectProperty("Bounds", "int Width", asOFFSET(Bounds, Width));
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_CONSTRUCT, "void bounds()", asFUNCTION(ConstructBasic<Bounds>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_CONSTRUCT, "void bounds(int h,int w)", asFUNCTION(ConstructBounds), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_DESTRUCT, "void bounds()", asFUNCTION(Destruct<Bounds>), asCALL_CDECL_OBJFIRST);
	//WINDOW REGISTRATION
	Engine->RegisterObjectType("Window", 0, asOBJ_REF | asOBJ_NOHANDLE);
	Engine->RegisterObjectMethod("Window", "int GetWidth()", asFUNCTION(getWindowWidth), asCALL_CDECL_OBJLAST);
	Engine->RegisterObjectMethod("Window", "int GetHeight()", asMETHOD(IWindow, GetHeight), asCALL_THISCALL); 
	Engine->RegisterObjectMethod("Window", "const Bounds GetBounds()", asMETHOD(IWindow, GetBounds), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Window", "void SetTitle(const string &in)", asFUNCTION(ChangeTitle), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectMethod("Window", "bool IsVsyncEnabled()", asMETHOD(IWindow, IsVsyncEnabled), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Window", "void Resize(int Width,int Height)", asMETHOD(IWindow, Resize), asCALL_THISCALL);
	//GAMEOBJECT REGSITRATION
	r = Engine->RegisterObjectType("GameObject", 0, asOBJ_REF); assert(r >= 0);
	r = Engine->SetDefaultNamespace("GameObject"); assert(r >= 0);
	Engine->RegisterEnum("Primitives");
	SpRegisterEnumValue(Engine, Primitives, Cube);
	SpRegisterEnumValue(Engine, Primitives, Plane);
	SpRegisterEnumValue(Engine, Primitives, Sphere);
	SpRegisterEnumValue(Engine, Primitives, Triangle);
	SpRegisterEnumValue(Engine, Primitives, Capsule);
	r = Engine->RegisterGlobalFunction("GameObject@ CreatePrimitive(Primitives)", asFUNCTION(GameObject::CreatePrimitive), asCALL_CDECL);
	assert(r >= 0);
	Engine->SetDefaultNamespace("Sphynx");
	Engine->RegisterObjectBehaviour("GameObject", asBEHAVE_ADDREF, "void addref()",
		asMETHOD(ScriptObjectWrapper<GameObject>, AddRef), asCALL_THISCALL);
	Engine->RegisterObjectBehaviour("GameObject", asBEHAVE_RELEASE, "void release()",
		asMETHOD(ScriptObjectWrapper<GameObject>, Release), asCALL_THISCALL);
	//SCENE REGSITRATION
	Engine->RegisterObjectType("Scene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	Engine->RegisterObjectBehaviour("Scene", asBEHAVE_FACTORY, "Scene@ s()", asFUNCTION(CreateBasic<Scene>), asCALL_CDECL);
	Engine->RegisterObjectMethod("Scene", "string& GetName()", asMETHOD(Scene, GetName), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Scene", "void SetName(const string &in)", asMETHOD(Scene, SetName), asCALL_THISCALL);
	//SCENEMANAGER REGSITRATION
	Engine->SetDefaultNamespace("Sphynx::SceneManager");
	Engine->RegisterGlobalFunction("Scene@ GetCurrentScene()", asFUNCTION(SceneManager::GetScene), asCALL_CDECL);
	r = Engine->RegisterGlobalFunction("void AddScene(Scene@ s)", asFUNCTION(SceneManager::AddScene), asCALL_CDECL); assert(r >= 0);
	Engine->SetDefaultNamespace("Sphynx");
	Engine->SetDefaultNamespace("");
}

Sphynx::Core::Scripting::AngelScript::~AngelScript()
{
	Engine->ShutDownAndRelease();
}

void Sphynx::Core::Scripting::AngelScript::CreateScript(AsScript* script, std::string& Code, const char* SectionName, const char* ModuleName)
{
	int r = builder.StartNewModule(Engine, ModuleName);
	if (r < 0)
	{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		Core_Error("Unrecoverable error while starting a new module.");
		return;
	}
}

void Sphynx::Core::Scripting::AngelScript::CreateScript(AsScript* script, const char* path, const char* ModuleName)
{
	int r = builder.StartNewModule(Engine, ModuleName);
	if (r < 0)
	{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		Core_Error("Unrecoverable error while starting a new module.");
		return;
	}
	r = builder.AddSectionFromFile(path);
	if (r < 0)
	{
		// The builder wasn't able to load the file.
		Core_Error("File Wasn't Loaded, Please correct the errors in the script and try again.");
		return;
	}
	r = builder.BuildModule();
	if (r < 0)
	{
		// An error occurred. Instruct the script writer to fix the 
		// compilation errors that were listed in the output stream.
		Core_Error("Could Not Build Module, Please correct the errors in the script and try again.");
		return;
	}
	script->Module = Engine->GetModule(ModuleName);
	script->Context = Engine->CreateContext();
}

void Sphynx::Core::Scripting::AsScript::OnComponentAttach(GameObject* parent)
{
	Run();
}

void Sphynx::Core::Scripting::AsScript::OnComponentDetach()
{
}

void Sphynx::Core::Scripting::AsScript::Update()
{
}

Sphynx::Core::Scripting::AsScript::AsScript(const char* path, const char* ModuleName)
{
	ScriptingEngine::GetAngelScript().CreateScript(this, path, ModuleName);
}

Sphynx::Core::Scripting::AsScript::~AsScript()
{
	Context->Release();
}

void Sphynx::Core::Scripting::AsScript::Run()
{
	if (!Module || !Context)return;
	asIScriptFunction* func = Module->GetFunctionByDecl("void main()");
	if (func == 0)
	{
		// The function couldn't be found. Instruct the script writer
		// to include the expected function in the script.
		Core_Error("The script must have the function 'void main()'. Please add it and try again.");
		return;
	}
	Context->Prepare(func);
	auto r = Context->Execute();
	if (r != asEXECUTION_FINISHED)
	{
		// The execution didn't complete as expected. Determine what happened.
		if (r == asEXECUTION_EXCEPTION)
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			Core_Error("An exception '{0}' occurred. Please correct the code and try again.\n", Context->GetExceptionString());
		}
	}
}
