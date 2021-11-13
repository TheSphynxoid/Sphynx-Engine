#pragma once

//--------------OS identificaton-------------------------
//Mac won't be supported.
/* Windows x64/x86 */
#ifdef _WIN32
	/* Windows x64  */
	#ifdef _WIN64
		#define Platform_Windows
		//#define WIN32_LEAN_AND_MEAN
		//#include <Windows.h>
		//#include <d3d.h>
		//#include <d2d1.h>
	#else
		/* Windows x86 */
		#error "x86 Builds are not supported!"
	#endif
#elif defined(__linux__)
	#define Platform_Linux
#else
	#error "No plans to support other platform (MacOS,IOS will never be supported by me as i can't be bothered to learn metal or Apple bullshit)"
#endif

//For Application Use only
#include "Sphynx/Application.h"
#include "Sphynx/Core/Graphics/Window.h"
#include "Sphynx/Logger.h"
#include "Sphynx/Events/Event.h"
#include "Sphynx/Core/Graphics/Imgui.h"
//Exposing Imgui on its own.
#ifdef EXPOSE_IMGUI_H
#include "../dep/imgui/imgui.h"
#endif // EXPOSE_IMGUI_H
//Exposing Imgui Implementation Headers.
#ifdef EXPOSE_IMGUI_IMPL
#ifdef IMGUI_GLFW
#include "Sphynx/imgui/imgui_impl_opengl3.h"
#include "Sphynx/imgui/imgui_impl_glfw.h"
#elif defined(IMGUI_DX11) && !defined(IMGUI_GLFW)
#include "Sphynx/imgui/imgui_impl_win32.h"
#include "Sphynx/imgui/imgui_impl_dx11.h"
#else
#include "Sphynx/imgui/imgui_impl_opengl3.h"
#include "Sphynx/imgui/imgui_impl_glfw.h"
#include "../dep/glfw/include/GLFW/glfw3.h"
#endif
#endif//EXPOSE_IMGUI_IMPL
//-------------------------------------------------------------
#include "GameObject.h"
#include "Component.h"
#define Time_Header "SpTime.h"
#include Time_Header
#include "Delegate.h"
#include "Input.h"
#include "Events/InputEvents.h"
//--------------------Globals-----------------------------------
//--------------------EntryPoint Header-------------------------
#include "Sphynx/EntryPoint.h"