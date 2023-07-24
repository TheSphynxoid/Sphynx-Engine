-- rule "AngelScriptRule"
-- 	display "AngelScriptRule"
-- 	fileextension ".as"
workspace "NewSphynx"
	architecture "x86_64"
	startproject "Sandbox"
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
		"Sphynx/src/**.as"
	}
	includedirs
	{
		"%{prj.name}/dep/spdlog/include",
		"%{prj.name}/src/Sphynx",
		"%{prj.name}/dep/glfw/include",
		"%{prj.name}/dep/glad/include",
		"%{prj.name}/dep/imgui",
		"%{prj.name}/dep/glm",
		"%{prj.name}/dep/stb",
		"%{prj.name}/dep/angelscript/sdk/angelscript/include",
		"%{prj.name}/dep/angelscript/sdk/add_on",
		"%{prj.name}/dep/mono/include"
	}
	links
	{
		"glfw",
		"glad",
		"imgui",
		"opengl32.lib",
		"angelscript",
		"%{prj.name}/dep/mono/lib/libmono-static-sgen.lib"
	}
	defines{
		-- "SPDLL"
	}
	filter "files:**.as"
		buildaction "Copy"
	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"
		-- links{ "%{wks.location}/%{prj.name}/dep/angelscript/sdk/angelscript/lib/angelscriptd.lib" }

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"
		-- links{ "%{wks.location}/%{prj.name}/dep/angelscript/sdk/angelscript/lib/angelscript.lib" }

	filter "system:windows"
		systemversion "latest"
	filter{}

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
		"%{wks.location}/Sphynx/dep/glm",
		"%{wks.location}/Sphynx/src/Sphynx",
		"%{wks.location}/Sphynx/src",
		"%{prj.name}/dep/angelscript/sdk/angelscript/include",
		"%{prj.name}/dep/angelscript/sdk/add_on"
	}

	links
	{
		"Sphynx",
		--I think I am force to do this.
		"AngelScript"
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
include "Sphynx/dep/imgui"
include "Sphynx/dep/AngelScript.lua"