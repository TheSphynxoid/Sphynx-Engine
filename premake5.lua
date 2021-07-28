workspace "NewSphynx"
	architecture "x86_64"
	configurations  
	{
		"Debug", "Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sphynx"
	location "Sphynx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	pchheader "pch.h"
	pchsource "Sphynx/src/pch.cpp"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Sphynx/src/**.h",
		"Sphynx/src/**.cpp",
		"Sphynx/src/**.lua"
	}
	includedirs
	{
		"%{prj.name}/dep/spdlog/include",
		"%{prj.name}/src/Sphynx",
		"%{prj.name}/dep/glfw/include",
		"%{prj.name}/dep/glad/include",
		"%{prj.name}/dep/imgui",
		"%{prj.name}/dep/glm",
		"%{prj.name}/dep/lua/src"
	}
	links
	{
		"glfw",
		"glad",
		"imgui",
		"Lua",
		"opengl32.lib"
	}
	defines{
		-- "SPDLL"
	}
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"


	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

	filter "system:windows"
		systemversion "latest"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Sandbox/src/**.h",
		"Sandbox/src/**.cpp",
		"Sandbox/src/**.lua"
	}

	includedirs
	{
		"%{wks.location}/Sphynx/dep/spdlog/include",
		"%{wks.location}/Sphynx/src/Sphynx",
		"%{wks.location}/Sphynx/src"
	}

	links
	{
		"Sphynx",
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"

include "Sphynx/dep/glfw"
include "Sphynx/dep/glad/glad"
<<<<<<< HEAD
include "Sphynx/dep/imgui"
=======
include "Sphynx/dep/imgui"
include "Sphynx/dep/Lua"
>>>>>>> Dev_ComponentSystem
