#ifndef Sphynx_Exceptions
#define Sphynx_Exceptions
#include <exception>
#include "Logger.h"
namespace Sphynx::Exception {
#ifdef Mem_Excep
	class OutOfMemory : public std::exception {
	public:
		OutOfMemory(const char* message) : exception(message) {
			Core_Warn("Out of memory should be handled.");
		}
	};
#endif
	class NotImplemented : public std::exception {
	public:
		NotImplemented(const char* msg) : std::exception(msg) {
#ifdef Platform_Windows
			DebugBreakProcess(GetCurrentProcess());
#endif
		}
	};
#ifdef App_Excep
	//Aborts the application when the .ctor gets called.
	class Brick : std::exception {
	public:
		Brick(const char* message){
			Core_Error(message);
			std::abort();
		}
	};
#endif
}
#else
#endif