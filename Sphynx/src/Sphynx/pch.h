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
#include "BitBool.h"
#define BIT(i) (1 << i)
struct DataBuffer {
	void* Data;
	size_t Size;
};
#define SPH_Move(...)	static_cast<std::remove_reference_t<decltype(__VA_ARGS__)>&&>(__VA_ARGS__)
#define SPH_FORWARD(...) static_cast<decltype(__VA_ARGS__)&&>(__VA_ARGS__)
template<class ...Args>
bool RuntimeInit(void(*f)(Args... a), Args&&... args) {
	f(SPH_FORWARD(args)...);
	return true;
}
#else
#endif