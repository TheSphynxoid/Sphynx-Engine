#pragma once
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cassert>
#include "PlatformMacros.h"
#ifdef Platform_Win
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(__linux__)
#endif
//Sphynx Libraries
#include "Logger.h"