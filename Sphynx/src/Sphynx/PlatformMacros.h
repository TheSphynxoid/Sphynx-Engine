#ifndef PlatformHeader
#define PlatformHeader
namespace Sphynx {
	enum class Platform : int {
		Error, Windows, Linux, Mac, Android
	};
//Mac won't be supported.
/* Windows x64/x86 */
#if defined(_WIN32)
/* Windows x64  */
#if defined(_WIN64)
#define Platform_Windows 1
	constexpr const Platform CurrentPlatform = Platform::Windows;
	constexpr const Platform GetPlatform() { return CurrentPlatform; };
/* Windows x86 */
#else
#error "x86 Builds are not supported!"
#endif
#elif defined(__linux__)
#define Platform_Linux 1
	constexpr const Platform CurrentPlatform = Platform::Linux;
	constexpr const Platform GetPlatform() { return CurrentPlatform; };
#endif
}
#else
#endif