#pragma once

//Microsoft Windows Platform
#ifdef Platform_Win
	#ifdef BuildDLL
		#define SphynxAPI __declspec(dllexport)
	#else
		#define SphynxAPI __declspec(dllimport)
	#endif // BuildDLL
#else
	#define SphynxAPI
#endif // Platform_Win
