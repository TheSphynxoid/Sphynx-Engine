project "ScriptAssembly"
	kind "SharedLib"
	language "C#"
	dotnetframework "4.6"
	architecture "x86_64"
	files
	{
		"**.cs"
	}
	flags{
		NoIncrementalLink
	}
	namespace ("Sphynx")
	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")