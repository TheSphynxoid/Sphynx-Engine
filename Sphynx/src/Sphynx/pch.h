#pragma once
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include "PlatformMacros.h"
#ifdef Platform_Win
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(__linux__)
#endif