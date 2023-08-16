#pragma once

namespace Sphynx::Core::Scripting {
	class ScriptingBackend {
	private:
	public:
		ScriptingBackend() = default;
		virtual ~ScriptingBackend() = default;
		virtual void Start() = 0;
		virtual void Update() = 0;
		virtual void Shutdown() = 0;
	};
}