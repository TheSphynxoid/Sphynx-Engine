project "glad"
    kind "StaticLib"
    language "C"

    targetdir ("%{wks.location}/build/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/build/int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "src/glad.c",
        "include/glad/glad.h",
        "include/KHR/khrplatform.h"
    }
    includedirs
    {
        "include"
    }