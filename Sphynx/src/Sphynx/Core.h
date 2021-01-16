#pragma once

#ifdef Platform_Win
	#ifdef BuildDLL
		#define SphynxAPI __declspec(dllexport)
	#else
		#define SphynxAPI __declspec(dllimport)
	#endif // BuildDLL
#endif // Platform_Win
