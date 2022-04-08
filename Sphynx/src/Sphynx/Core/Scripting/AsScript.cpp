#include "pch.h"
#include "AsScript.h"
#include "ScriptingEngine.h"
#include "Application.h"
#include "Core/Graphics/Window.h"
#include "Scene.h"
#include "Core/SceneManager.h"
#include "Core/Threadpool.h"
#include "Input.h"
#include "BaseLib/AsBaseLib.h"
#include "AssetManager.h"
#include "Core/Graphics/Pipeline/Texture.h"
#include "Core/Graphics/Pipeline/FrameBuffer.h"
#include <scriptstdstring/scriptstdstring.h>
#include <datetime/datetime.h>
#include <scriptarray/scriptarray.h>
#include <scriptany/scriptany.h>
#include <scriptfile/scriptfile.h>
#undef DeleteFile
#undef CopyFile
#include <scriptfile/scriptfilesystem.h>
//Oh No...
//This is Because We Don't Compile The Extentions with AngelScript.
#include <scriptstdstring/scriptstdstring.cpp>
#include <datetime/datetime.cpp>
#include <scriptarray/scriptarray.cpp>
#include <scriptbuilder/scriptbuilder.cpp>
#include <scriptany/scriptany.cpp>
#include <scriptfile/scriptfile.cpp>
#include <scriptfile/scriptfilesystem.cpp>

#undef Path

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
	auto r = Engine->RegisterGlobalProperty("Sphynx::Application App", GetApplication()); assert(r >= 0);
}

void Sphynx::Core::Scripting::AngelScript::OnWindowOpen(Events::OnWindowOpen& e)
{
	auto r = Engine->RegisterGlobalProperty("Sphynx::Window MainWindow", GetMainWindow());
	assert(r >= 0);
}

int getWindowWidth() {
	return GetMainWindow()->GetWidth();
}

template<class T>
void ConstructBasic(void* mem)
{
	new(mem)T();
}

template<class T>
T* CreateBasic() {
	T* t = new T();
	return t;
}

template<class T, typename ...Args>
void ConstructArgs(void* mem, Args ...args) {
	new(mem)T(args...);
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

void CreateWindowAS(Application* app, std::string title, Bounds b, bool fullscreen) {
	app->CreateMainWindow(IWindow::Create(app, b, title, fullscreen));
}

Bounds GetTextureBounds(Texture* tex) {
	return { tex->GetWidth(), tex->GetHeight() };
}

Camera* GetCamera() {
	return SceneManager::GetScene().GetPrimaryCamera();
}

int IncludeCallBack(const char* include, const char* from, CScriptBuilder* builder, void* UserParam) {
	if (strcmp(include, from) == 0) {
		Core_Error("{0} Script Includes itself", from);
		return asERetCodes::asERROR;
	}
	else if(strstr(include,".as")) {
		return builder->AddSectionFromFile(include);
	}
	/*else {
		builder->AddSectionFromMemory(include, Libs[include]);
	}*/
}

bool IsKeyPressed(int _key) {
	return Input::IsKeyPressed((Keys)_key);
}
bool IsMouseButtonPressed(int _button) {
	return Input::IsMouseButtonPressed((MouseButton)_button);
}
void RegisterInput(asIScriptEngine* engine) {
	engine->SetDefaultNamespace("Sphynx");
#pragma region Keys Enum
	engine->RegisterEnum("Keys");
	SpRegisterEnumValue(engine, Keys, Space);
	SpRegisterEnumValue(engine, Keys, Apostrophe);
	SpRegisterEnumValue(engine, Keys, Comma);
	SpRegisterEnumValue(engine, Keys, Minus);
	SpRegisterEnumValue(engine, Keys, Period);
	SpRegisterEnumValue(engine, Keys, Slash);
	SpRegisterEnumValue(engine, Keys, Num0);
	SpRegisterEnumValue(engine, Keys, Num1);
	SpRegisterEnumValue(engine, Keys, Num2);
	SpRegisterEnumValue(engine, Keys, Num3);
	SpRegisterEnumValue(engine, Keys, Num4);
	SpRegisterEnumValue(engine, Keys, Num5);
	SpRegisterEnumValue(engine, Keys, Num6);
	SpRegisterEnumValue(engine, Keys, Num7);
	SpRegisterEnumValue(engine, Keys, Num8);
	SpRegisterEnumValue(engine, Keys, Num9);
	SpRegisterEnumValue(engine, Keys, SemiColon);
	SpRegisterEnumValue(engine, Keys, Equal);
	SpRegisterEnumValue(engine, Keys, A);
	SpRegisterEnumValue(engine, Keys, B);
	SpRegisterEnumValue(engine, Keys, C);
	SpRegisterEnumValue(engine, Keys, D);
	SpRegisterEnumValue(engine, Keys, E);
	SpRegisterEnumValue(engine, Keys, F);
	SpRegisterEnumValue(engine, Keys, G);
	SpRegisterEnumValue(engine, Keys, H);
	SpRegisterEnumValue(engine, Keys, I);
	SpRegisterEnumValue(engine, Keys, J);
	SpRegisterEnumValue(engine, Keys, K);
	SpRegisterEnumValue(engine, Keys, L);
	SpRegisterEnumValue(engine, Keys, M);
	SpRegisterEnumValue(engine, Keys, N);
	SpRegisterEnumValue(engine, Keys, O);
	SpRegisterEnumValue(engine, Keys, P);
	SpRegisterEnumValue(engine, Keys, Q);
	SpRegisterEnumValue(engine, Keys, R);
	SpRegisterEnumValue(engine, Keys, S);
	SpRegisterEnumValue(engine, Keys, T);
	SpRegisterEnumValue(engine, Keys, U);
	SpRegisterEnumValue(engine, Keys, V);
	SpRegisterEnumValue(engine, Keys, W);
	SpRegisterEnumValue(engine, Keys, X);
	SpRegisterEnumValue(engine, Keys, Y);
	SpRegisterEnumValue(engine, Keys, Z);
	SpRegisterEnumValue(engine, Keys, LeftBracket);
	SpRegisterEnumValue(engine, Keys, BackSlash);
	SpRegisterEnumValue(engine, Keys, RightBracket);
	SpRegisterEnumValue(engine, Keys, GraveAccent);
	SpRegisterEnumValue(engine, Keys, World1);
	SpRegisterEnumValue(engine, Keys, World2);
	SpRegisterEnumValue(engine, Keys, Escape);
	SpRegisterEnumValue(engine, Keys, Enter);
	SpRegisterEnumValue(engine, Keys, Tab);
	SpRegisterEnumValue(engine, Keys, BackSpace);
	SpRegisterEnumValue(engine, Keys, Insert);
	SpRegisterEnumValue(engine, Keys, Delete);
	SpRegisterEnumValue(engine, Keys, Right);
	SpRegisterEnumValue(engine, Keys, Left);
	SpRegisterEnumValue(engine, Keys, Down);
	SpRegisterEnumValue(engine, Keys, Up);
	SpRegisterEnumValue(engine, Keys, PageUp);
	SpRegisterEnumValue(engine, Keys, PageDown);
	SpRegisterEnumValue(engine, Keys, Home);
	SpRegisterEnumValue(engine, Keys, End);
	SpRegisterEnumValue(engine, Keys, CapsLock);
	SpRegisterEnumValue(engine, Keys, ScrollLock);
	SpRegisterEnumValue(engine, Keys, NumLock);
	SpRegisterEnumValue(engine, Keys, PrintScreen);
	SpRegisterEnumValue(engine, Keys, Pause);
	SpRegisterEnumValue(engine, Keys, F1);
	SpRegisterEnumValue(engine, Keys, F2);
	SpRegisterEnumValue(engine, Keys, F3);
	SpRegisterEnumValue(engine, Keys, F4);
	SpRegisterEnumValue(engine, Keys, F5);
	SpRegisterEnumValue(engine, Keys, F6);
	SpRegisterEnumValue(engine, Keys, F7);
	SpRegisterEnumValue(engine, Keys, F8);
	SpRegisterEnumValue(engine, Keys, F9);
	SpRegisterEnumValue(engine, Keys, F10);
	SpRegisterEnumValue(engine, Keys, F11);
	SpRegisterEnumValue(engine, Keys, F12);
	SpRegisterEnumValue(engine, Keys, F13);
	SpRegisterEnumValue(engine, Keys, F14);
	SpRegisterEnumValue(engine, Keys, F15);
	SpRegisterEnumValue(engine, Keys, F16);
	SpRegisterEnumValue(engine, Keys, F17);
	SpRegisterEnumValue(engine, Keys, F18);
	SpRegisterEnumValue(engine, Keys, F19);
	SpRegisterEnumValue(engine, Keys, F20);
	SpRegisterEnumValue(engine, Keys, F21);
	SpRegisterEnumValue(engine, Keys, F22);
	SpRegisterEnumValue(engine, Keys, F23);
	SpRegisterEnumValue(engine, Keys, F24);
	SpRegisterEnumValue(engine, Keys, F25);
	SpRegisterEnumValue(engine, Keys, Kp0);
	SpRegisterEnumValue(engine, Keys, Kp1);
	SpRegisterEnumValue(engine, Keys, Kp2);
	SpRegisterEnumValue(engine, Keys, Kp3);
	SpRegisterEnumValue(engine, Keys, Kp4);
	SpRegisterEnumValue(engine, Keys, Kp5);
	SpRegisterEnumValue(engine, Keys, Kp6);
	SpRegisterEnumValue(engine, Keys, Kp7);
	SpRegisterEnumValue(engine, Keys, Kp8);
	SpRegisterEnumValue(engine, Keys, Kp9);
	SpRegisterEnumValue(engine, Keys, KpDecimal);
	SpRegisterEnumValue(engine, Keys, KpDivide);
	SpRegisterEnumValue(engine, Keys, KpMultiply);
	SpRegisterEnumValue(engine, Keys, KpSubtract);
	SpRegisterEnumValue(engine, Keys, KpAdd);
	SpRegisterEnumValue(engine, Keys, KpEnter);
	SpRegisterEnumValue(engine, Keys, KpEqual);
	SpRegisterEnumValue(engine, Keys, LeftShift);
	SpRegisterEnumValue(engine, Keys, LeftControl);
	SpRegisterEnumValue(engine, Keys, LeftAlt);
	SpRegisterEnumValue(engine, Keys, LeftSuper);
	SpRegisterEnumValue(engine, Keys, RightShift);
	SpRegisterEnumValue(engine, Keys, RightControl);
	SpRegisterEnumValue(engine, Keys, RightAlt);
	SpRegisterEnumValue(engine, Keys, RightSuper);
	SpRegisterEnumValue(engine, Keys, Menu);
#pragma endregion
	engine->SetDefaultNamespace("Sphynx::Input");
	int r = engine->RegisterGlobalFunction("bool IsKeyPressed(Sphynx::Keys)", asFUNCTION(IsKeyPressed), asCALL_CDECL);
}

std::string GetGOName(GameObject* go) {
	return std::string(go->GetName());
}

Sphynx::Core::Scripting::AngelScript::AngelScript()
{
	Events::GlobalEventSystem::GetInstance()->Subscribe<AngelScript, Events::OnApplicationStart>(this, &AngelScript::OnAppStart);
	Events::GlobalEventSystem::GetInstance()->Subscribe<AngelScript, Events::OnWindowOpen>(this, &AngelScript::OnWindowOpen);
	asPrepareMultithread();
	Engine = asCreateScriptEngine();
	Engine->SetMessageCallback(asFunctionPtr(&MessageCallback), NULL, asCALL_CDECL);
	Engine->SetEngineProperty(asEP_BUILD_WITHOUT_LINE_CUES, true);
	Engine->SetEngineProperty(asEP_REQUIRE_ENUM_SCOPE, true);
	Engine->SetEngineProperty(asEP_DISALLOW_GLOBAL_VARS, true);
	Engine->SetEngineProperty(asEP_ALWAYS_IMPL_DEFAULT_CONSTRUCT, true);
	//AngelScripts addons
	RegisterStdString(Engine);
	RegisterScriptAny(Engine);
	RegisterScriptArray(Engine, true);
	RegisterScriptDateTime(Engine);
	RegisterScriptFile(Engine);
	RegisterScriptFileSystem(Engine);
	//End AngelScript Addons
	Engine->SetDefaultAccessMask(0x3);
	Engine->RegisterGlobalFunction("void Print(const string &in)", asFUNCTION(Print), asCALL_CDECL);
	Engine->RegisterGlobalFunction("const double get_deltaTime() property", asFUNCTION(Time::GetDeltaTime), asCALL_CDECL);
	Engine->SetDefaultNamespace("Sphynx");
	//BOUNDS REGISTRATION
	Engine->RegisterObjectType("Bounds", sizeof(Bounds),
		asOBJ_VALUE | asOBJ_APP_CLASS_MORE_CONSTRUCTORS | asOBJ_POD | asGetTypeTraits<Bounds>());
	Engine->RegisterObjectProperty("Bounds", "int Height", asOFFSET(Bounds, Height));
	Engine->RegisterObjectProperty("Bounds", "int Width", asOFFSET(Bounds, Width));
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_CONSTRUCT, "void bounds()", asFUNCTION(ConstructBasic<Bounds>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_CONSTRUCT, "void bounds(int h,int w)", asFUNCTION(ConstructBounds), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Bounds", asBEHAVE_DESTRUCT, "void bounds()", asFUNCTION(Destruct<Bounds>), asCALL_CDECL_OBJFIRST);
	//VECTOR2 REGISTRATION
	Engine->RegisterObjectType("Vector2", sizeof(glm::vec2),
		asOBJ_VALUE | asOBJ_APP_CLASS_MORE_CONSTRUCTORS | asOBJ_POD | asGetTypeTraits<glm::vec2>());
	Engine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void v2()", asFUNCTION(ConstructBasic<glm::vec2>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector2", asBEHAVE_CONSTRUCT, "void v2(float x,float y)",
		asFunctionPtr(ConstructArgs<glm::vec2, float, float>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector2", asBEHAVE_DESTRUCT, "void v2()", asFUNCTION(Destruct<glm::vec2>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectProperty("Vector2", "float x", asOFFSET(glm::vec2, x));
	Engine->RegisterObjectProperty("Vector2", "float y", asOFFSET(glm::vec2, y));
	//VECTOR3 REGISTRATION
	Engine->RegisterObjectType("Vector3", sizeof(glm::vec3),
		asOBJ_VALUE | asOBJ_APP_CLASS_MORE_CONSTRUCTORS | asOBJ_POD | asGetTypeTraits<glm::vec3>());
	Engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void v2()", asFUNCTION(ConstructBasic<glm::vec3>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void v2(float x,float y,float z)",
		asFunctionPtr(ConstructArgs<glm::vec3, float, float, float>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector3", asBEHAVE_CONSTRUCT, "void v2(float scalar)", 
		asFunctionPtr(ConstructArgs<glm::vec3, float>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector3", asBEHAVE_DESTRUCT, "void v2()", asFUNCTION(Destruct<glm::vec3>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectProperty("Vector3", "float x", asOFFSET(glm::vec3, x));
	Engine->RegisterObjectProperty("Vector3", "float y", asOFFSET(glm::vec3, y));
	Engine->RegisterObjectProperty("Vector3", "float z", asOFFSET(glm::vec3, z));
	//VECTOR4 REGISTRATION
	Engine->RegisterObjectType("Vector4", sizeof(glm::vec4),
		asOBJ_VALUE | asOBJ_APP_CLASS_MORE_CONSTRUCTORS | asOBJ_POD | asGetTypeTraits<glm::vec4>());
	Engine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void v2()", asFUNCTION(ConstructBasic<glm::vec4>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector4", asBEHAVE_CONSTRUCT, "void v2(float x,float y,float z,float w)",
		asFunctionPtr(ConstructArgs<glm::vec4, float, float, float, float>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectBehaviour("Vector4", asBEHAVE_DESTRUCT, "void v2()", asFUNCTION(Destruct<glm::vec4>), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectProperty("Vector4", "float x", asOFFSET(glm::vec4, x));
	Engine->RegisterObjectProperty("Vector4", "float y", asOFFSET(glm::vec4, y));
	Engine->RegisterObjectProperty("Vector4", "float z", asOFFSET(glm::vec4, z));
	Engine->RegisterObjectProperty("Vector4", "float w", asOFFSET(glm::vec4, w));
	//WINDOW REGISTRATION
	Engine->RegisterObjectType("Window", 0, asOBJ_REF | asOBJ_NOHANDLE);
	Engine->RegisterObjectMethod("Window", "int get_Width() property", asFUNCTION(getWindowWidth), asCALL_CDECL_OBJLAST);
	Engine->RegisterObjectMethod("Window", "int get_Height() property", asMETHOD(IWindow, GetHeight), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Window", "const Bounds get_Bounds() property", asMETHOD(IWindow, GetBounds), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Window", "void SetTitle(const string &in)", asFUNCTION(ChangeTitle), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectMethod("Window", "bool IsVsyncEnabled()", asMETHOD(IWindow, IsVsyncEnabled), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Window", "void Resize(int Width,int Height)", asMETHOD(IWindow, Resize), asCALL_THISCALL);
	//APPLICATION REGISTRATION
	Engine->RegisterObjectType("Application", 0, asOBJ_REF | asOBJ_NOHANDLE);
	Engine->RegisterObjectMethod("Application", "bool HasWindow()", asMETHOD(Application, Application::HasWindow), asCALL_THISCALL);
	//GAMEOBJECT REGISTRATION
	//GAMEOBJECT
	Engine->RegisterObjectType("GameObject", 0, asOBJ_REF | asOBJ_NOCOUNT);
	Engine->RegisterObjectMethod("GameObject", "void Destroy()", asMETHOD(GameObject, Destroy), asCALL_THISCALL);
	Engine->RegisterObjectMethod("GameObject", "bool get_Active() property", asMETHOD(GameObject, IsActive), asCALL_THISCALL);
	Engine->RegisterObjectMethod("GameObject", "const string get_Name() property", asFUNCTION(GetGOName), asCALL_CDECL_OBJFIRST);
	Engine->RegisterObjectMethod("GameObject", "GameObject& opAssign(const GameObject& in)",
		asMETHODPR(GameObject, operator=, (const GameObject&), GameObject&), asCALL_THISCALL);
	Engine->RegisterObjectMethod("GameObject", "bool opEquals(const GameObject& in)",
		asMETHODPR(GameObject, operator==, (const GameObject&), bool), asCALL_THISCALL);
	Engine->RegisterGlobalProperty("const GameObject@ PlaceHolder", (void*)&GameObject::PlaceHolder);

	Engine->RegisterObjectType("Transform", 0, asOBJ_REF | asOBJ_NOCOUNT);
	Engine->RegisterObjectMethod("GameObject", "Transform@ get_transform() property", asMETHOD(GameObject, GetTransform), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Transform", "void Translate(Vector3)", asMETHOD(Transform, Translate), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Transform", "void MoveTo(Vector3)", asMETHOD(Transform, SetPosition), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Transform", "Vector3& get_Position() property", asMETHOD(Transform, GetPosition), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Transform", "GameObject@ get_gameObject() property", asMETHOD(Transform, GetGameObject), asCALL_THISCALL);
	//SCENE REGISTRATION
	Engine->RegisterObjectType("Scene", 0, asOBJ_REF | asOBJ_NOCOUNT);
	Engine->RegisterObjectBehaviour("Scene", asBEHAVE_FACTORY, "Scene@ s()", asFUNCTION(CreateBasic<Scene>), asCALL_CDECL);
	Engine->RegisterObjectMethod("Scene", "string& GetName()", asMETHOD(Scene, GetName), asCALL_THISCALL);
	Engine->RegisterObjectMethod("Scene", "void SetName(const string &in)", asMETHOD(Scene, SetName), asCALL_THISCALL);
	//SCENEMANAGER REGISTRATION
	Engine->SetDefaultNamespace("Sphynx::SceneManager");
	Engine->RegisterGlobalFunction("Scene@ GetCurrentScene()", asFUNCTION(SceneManager::GetScene), asCALL_CDECL);
	Engine->RegisterGlobalFunction("void AddScene(Scene@ s)", asFUNCTION(SceneManager::AddScene), asCALL_CDECL);
	RegisterInput(Engine);
	//Internal
	Engine->SetDefaultNamespace("Sphynx::Core");
	Engine->SetDefaultAccessMask(0x2);
	Engine->RegisterInterface("IComponent");
	Engine->SetDefaultNamespace("");
	builder.SetIncludeCallback(IncludeCallBack, NULL);
}

Sphynx::Core::Scripting::AngelScript::~AngelScript()
{
	Engine->ShutDownAndRelease();
}

void Sphynx::Core::Scripting::AngelScript::CreateScript(AsScript* script, std::string& Code, const char* sectionName, const char* ModuleName)
{
	int r = builder.StartNewModule(Engine, ModuleName);
	if (r < 0)
	{
		// If the code fails here it is usually because there
		// is no more memory to allocate the module
		Core_Error("Unrecoverable error while starting a new module.");
		return;
	}
	r = builder.AddSectionFromMemory(sectionName, Code.c_str(), Code.size(), 0);
	if (r < 0)
	{
		Core_Error("Section was already included, Please correct the errors in the script and try again.");
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
	script->SectionName = sectionName;
	script->Module = Engine->GetModule(ModuleName);
	script->Context = Engine->RequestContext();
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
	builder.GetModule()->SetAccessMask(0x2);
	r = builder.AddSectionFromFile("Component.as");
	if (r < 0) {
		Core_Error("Unable to Add Component.as");
		return;
	}
	builder.GetModule()->SetAccessMask(0x1);
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
	script->SectionName = path;
	script->Module = Engine->GetModule(ModuleName);
	script->Context = Engine->RequestContext();
}

asITypeInfo* Sphynx::Core::Scripting::AngelScript::GetSystemClassTypeInfo(const char* name)
{
	return Engine->GetTypeInfoByName(name);
}

asIScriptObject* Sphynx::Core::Scripting::AngelScript::CreateScriptObject(const asITypeInfo* type, ScriptBehaviour* Behaviour)
{
	auto obj = (asIScriptObject*)Engine->CreateScriptObject(type);
	Behaviour->TypeInfo = type;
	Behaviour->ScriptObject = obj;
	Behaviour->Name = type->GetName();
	return obj;
}

void Sphynx::Core::Scripting::AngelScript::ReloadScript(AsScript* script)
{
	Core_Info("Reloading {0} Script", script->GetScriptBehaviour().GetTypeInfo()->GetName());
	//Clean up
	Engine->ReturnContext(script->Context);
	script->Module->Discard();
	Engine->ReleaseScriptObject(script->scriptBehaviour.ScriptObject, script->scriptBehaviour.TypeInfo);
	//Loading the script.
	CreateScript(script, script->Path, script->ModuleName);
	script->Run();
}

asIScriptEngine* Sphynx::Core::Scripting::AsScript::GetEngine()
{
	return Sphynx::Core::Scripting::ScriptingEngine::GetAngelScript().Engine;
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
	if (Context) {
		ThreadPool::Submit([this]() {
			auto r = Context->Execute();
			if (r != asEXECUTION_FINISHED)
			{
				// The execution didn't complete as expected. Determine what happened.
				if (r == asEXECUTION_EXCEPTION)
				{
					// An exception occurred, let the script writer know what happened so it can be corrected.
					Core_Error("An exception '{0}' occurred. Please correct the code and try again.\n", Context->GetExceptionString());
					Context->GetEngine()->ReturnContext(Context);
					Context = nullptr;
					return;
				}
			}
			Context->Prepare(UpdateFunc);
			Context->SetObject(scriptBehaviour.ScriptObject);
		});
	}
}

Sphynx::Core::Scripting::AsScript::AsScript(const char* path, const char* moduleName) : Path(path), ModuleName(moduleName)
{
	ScriptingEngine::GetAngelScript().CreateScript(this, path, ModuleName);
}

Sphynx::Core::Scripting::AsScript::~AsScript()
{
	GetEngine()->ReturnContext(Context);
	Module->Discard();
	GetEngine()->ReleaseScriptObject(scriptBehaviour.ScriptObject, scriptBehaviour.TypeInfo);
}

void Sphynx::Core::Scripting::AsScript::Run()
{
	if (!Module || !Context)return;
	scriptBehaviour.Holder = this->GetGameObject();
	//TODO:This Will Get Buggy as More Complexe Scripts appear.
	scriptBehaviour.TypeInfo = Module->GetObjectTypeByIndex(Module->GetObjectTypeCount() - 1);
	scriptBehaviour.Name = scriptBehaviour.TypeInfo->GetName();
	asIScriptFunction* func = scriptBehaviour.TypeInfo->GetFactoryByIndex(0);
	Context->Prepare(func);
	auto r = Context->Execute();
	if (r != asEXECUTION_FINISHED)
	{
		// The execution didn't complete as expected. Determine what happened.
		if (r == asEXECUTION_EXCEPTION)
		{
			// An exception occurred, let the script writer know what happened so it can be corrected.
			Core_Error("An exception '{0}' occurred at \"{1}\" Line {2}. Please correct the code and try again.",
				Context->GetExceptionString(), Context->GetExceptionFunction()->GetName(), Context->GetExceptionLineNumber());
			Context->GetEngine()->ReturnContext(Context);
			Context = nullptr;
			return;
		}
		else if (r < 0) {
			Core_Error("Could Not Execute Component Constructor");
			Context->GetEngine()->ReturnContext(Context);
			Context = nullptr;
			return;
		}
	}
	scriptBehaviour.ScriptObject = (asIScriptObject*)Context->GetReturnObject();
	scriptBehaviour.ScriptObject->SetUserData(&scriptBehaviour, 1);
	(*((GameObject**)scriptBehaviour.ScriptObject->GetAddressOfProperty(0))) = scriptBehaviour.GetGameObject();
	(*((size_t*)scriptBehaviour.ScriptObject->GetAddressOfProperty(1))) = this->GetID();
	scriptBehaviour.ScriptObject->AddRef();
	//auto func = scriptBehaviour.TypeInfo->GetMethodByDecl("void Awake()");
	//if (func != 0)
	//{
	//	Context->Prepare(func);
	//	Context->SetObject(scriptBehaviour.ScriptObject);
	//	auto r = Context->Execute();
	//	if (r != asEXECUTION_FINISHED)
	//	{
	//		// The execution didn't complete as expected. Determine what happened.
	//		if (r == asEXECUTION_EXCEPTION)
	//		{
	//			// An exception occurred, let the script writer know what happened so it can be corrected.
	//			Core_Error("An exception '{0}' occurred at \"{1}\" Line {2}. Please correct the code and try again.",
	//				Context->GetExceptionString(), Context->GetExceptionFunction()->GetName(), Context->GetExceptionLineNumber());
	//			Context->GetEngine()->ReturnContext(Context);
	//			Context = nullptr;
	//			return;
	//		}
	//	}
	//}
	func = scriptBehaviour.TypeInfo->GetMethodByDecl("void Start()");
	if (func != 0)
	{
		Context->Prepare(func);
		Context->SetObject(scriptBehaviour.ScriptObject);
		r = Context->Execute();
		if (r != asEXECUTION_FINISHED)
		{
			// The execution didn't complete as expected. Determine what happened.
			if (r == asEXECUTION_EXCEPTION)
			{
				// An exception occurred, let the script writer know what happened so it can be corrected.
				Core_Error("An exception '{0}' occurred at \"{1}\" Line {2}. Please correct the code and try again.", 
					Context->GetExceptionString(), Context->GetExceptionFunction()->GetName(), Context->GetExceptionLineNumber());
				Context->GetEngine()->ReturnContext(Context);
				Context = nullptr;
				return;
			}
		}
	}
	UpdateFunc = scriptBehaviour.TypeInfo->GetMethodByDecl("void Update()");
	if (UpdateFunc) {
		Context->Prepare(UpdateFunc);
		Context->SetObject(scriptBehaviour.ScriptObject);
	}
}
