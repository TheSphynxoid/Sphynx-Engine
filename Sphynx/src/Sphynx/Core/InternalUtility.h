#pragma once
#include "Events/Event.h"
#include "Logger.h"
#include "Pointer.h"
#include "Memory.h"
//#define NotImplementedFunction(name,...) name(__VA_ARGS__){Core_Warn(#name, " not implemented");};
#define NotImplementedFunction(sig) sig{Core_Warn(#sig, " not implemented");};
namespace Sphynx::Util {
	void InternalSkipCurrentEvent(Events::EventSystem& es) {
		es.SkipCurrentEvent();
	}
};