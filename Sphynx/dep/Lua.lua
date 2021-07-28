project "Lua"
	kind "StaticLib"
	language "C"
	architecture "x86_64"
    staticruntime "On"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

	includedirs { "Lua/src" }

    files
    {
        "Lua/src/**.c",
        "Lua/src/**.hpp",
        "Lua/src/**.h"
    }
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        runtime "Release"
        optimize "on"
