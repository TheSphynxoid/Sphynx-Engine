#pragma once

//Microsoft Windows Platform
#ifdef Platform_Win|| _Win32|| __Win32__
	#ifdef BuildDLL
		#define SphynxAPI __declspec(dllexport)
	#else
		#define SphynxAPI __declspec(dllimport)
	#endif // BuildDLL
#endif // Platform_Win
