#ifndef Sphynx_Exceptions
#define Sphynx_Exceptions
#include <exception>
#include "Logger.h"
namespace Sphynx::Exception {
	class OutOfMemory : public std::exception {
	public:
		OutOfMemory(const char* message) : exception(message) {
			Core_Warn("Out of memory should be handled.");
		}
	};
	//Aborts the application when the .ctor gets called.
	class Brick : std::exception {
	public:
		Brick(const char* message){
			Core_Error(message);
			std::abort();
		}
	};
}
#else
#endif