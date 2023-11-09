#pragma once
#include "Component.h"

namespace Sphynx::Core::Scripting {
	class Script
	{
	public:
		virtual void Start() = 0;
		virtual const char* GetName() const noexcept = 0;
	};
}