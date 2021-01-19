workspace "NewSphynx"
	architecture "x86_64"
	configurations  
	{
		"Debug", "Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Sphynx"
	location "Sphynx"
	kind "SharedLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Sphynx/src/**.h",
		"Sphynx/src/**.cpp"
	}
	includedirs
	{
		"%{prj.name}/dep/spdlog/include"
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
		defines
		{
			Platform_Win
		}

project "Sandbox"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{wks.location}/Sandbox/src/**.h",
		"%{wks.location}/Sandbox/src/**.cpp"
	}

	includedirs
	{
		"%{wks.location}/Sphynx/dep/spdlog/include",
		"%{wks.location}/Sphynx/src"
	}

	links
	{
		"Sphynx"
	}

	filter "configurations:Debug"
		defines "DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "RELEASE"
		runtime "Release"
		optimize "on"