#pragma once
#ifndef Sphynx_PCH
#define Sphynx_PCH
#include <list>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <cassert>
#include <cstddef>
//#include "Core/Memory.h"
#include "PlatformMacros.h"
#ifdef Platform_Windows
	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
#elif defined(__linux__)
#endif
//Sphynx Libraries
#include "Logger.h"
#include "Core/IO/Serializer.h"
#define BIT(i) (1 << i)
struct Buffer {
	void* Data;
	size_t Size;
};
#else
#endif