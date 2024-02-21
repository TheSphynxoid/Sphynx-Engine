project "GameAssembly"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.8"
	architecture "x86_64"
	csversion "11"
	debuggertype "Mixed"
	files
	{
		"**.cs"
	}
	flags{
		NoIncrementalLink
	}
	links{
		"ScriptAssembly"
	}
	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")