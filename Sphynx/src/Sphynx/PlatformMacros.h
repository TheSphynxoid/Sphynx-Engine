#pragma once
namespace Sphynx {
	enum class Platform : int {
		Error, Windows, Linux, Mac, Android
	};
//Mac won't be supported.
/* Windows x64/x86 */
#ifdef _WIN32
/* Windows x64  */
#ifdef _WIN64
#define Platform_Windows 1
	constexpr const Platform CurrentPlatform = Platform::Windows;
	constexpr const Platform GetPlatform() { return CurrentPlatform; };
#else
/* Windows x86 */
#error "x86 Builds are not supported!"
#endif
#elif defined(__linux__)
#define Platform_Linux 1
	constexpr const Platform CurrentPlatform = Platform::Linux;
	constexpr const Platform GetPlatform() { return CurrentPlatform; };
#endif
}
