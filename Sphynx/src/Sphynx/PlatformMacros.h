#pragma once

//Mac won't be supported.
/* Windows x64/x86 */
#ifdef _WIN32
	/* Windows x64  */
	#ifdef _WIN64
		#define Platform_Win 1
	#else
	/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__linux__)
	#define Platform_Linux 1
#endif


enum class Platform : int {
	Error, Windows, Linux, Mac, Android
};