#pragma once
#ifndef Sphynx_PCH
#define Sphynx_PCH
#include <list>
#include <map>
#include <vector>
#include <memory>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cassert>
#include <cstddef>
#include "PlatformMacros.h"
#ifdef Platform_Windows
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(__linux__)
#endif
//Sphynx Libraries
#include "Logger.h"
#define BIT(i) (1 << i)
#else
#endif