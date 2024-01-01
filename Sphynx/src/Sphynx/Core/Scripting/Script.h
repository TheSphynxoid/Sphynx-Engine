#pragma once
#include "Component.h"


namespace Sphynx::Core::Scripting {
	class ScriptingBackend;
	class Script
	{
	public:
		virtual void Awake() = 0;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void FixedUpdate() = 0;
		virtual void OnDestroy() = 0;
		virtual Script* Copy() = 0;
		//Returns the native script live object. Possibly platform specific.
		//	Mono Specific: returns MonoObject*.
		virtual void* GetNativeObject() = 0;
		//If I implement other languages and this isn't used by them, I will remove it and refactor the mono implementation.
		//Returns the native script class representation. Possibly platform specific.
		//	Mono Specific: returns MonoClass*.
		virtual void* GetNativeClass() = 0;
		virtual const char* GetName() const noexcept = 0;
	};
}