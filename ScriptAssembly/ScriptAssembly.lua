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
	GAC = "%{wks.location}" .. "SandBox\\mono\\gac"
	links
	{
		GAC .. "Microsoft.Build\\14.0.0.0__b03f5f7f11d50a3a\\Microsoft.Build.dll",
		GAC .. "Mono.CSharp\\4.0.0.0__0738eb9f132ed756\\Mono.CSharp.dll",
		GAC .. "Mono.Cecil\\0.11.1.0__0738eb9f132ed756\\Mono.Cecil.dll",
		GAC .. "Mono.Parallel\\4.0.0.0__0738eb9f132ed756\\Mono.Parallel.dll",
	}
	namespace ("Sphynx")
	targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")