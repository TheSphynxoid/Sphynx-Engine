#pragma once

//Microsoft Windows Platform
#ifdef dPlatform_Win
	#ifdef BuildDLL
		#define SphynxAPI __declspec(dllexport)
	#else
		#define SphynxAPI __declspec(dllimport)
	#endif // BuildDLL
#elif Platform_Linux
	#ifdef DYNAMICLIB_EXPORTS     
		#define SphynxAPI __attribute__((visibility("default")))  
	#else     
		#define SphynxAPI
	#endif 
#endif// Platform_Win
