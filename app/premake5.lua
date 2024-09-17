project "App"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++20"
    systemversion "latest"

    targetdir("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
    
    include "linker.lua"

    files 
    {
        "src/**.h",
        "src/**.cpp"
    }

filter "configurations:Debug"
    defines 
    { 
        "BK_DEBUG",
        "DEBUG"
    }
    runtime "Debug"
    symbols "on"


filter "configurations:Release"
    defines 
    { 
        "BK_RELEASE",
        "NDEBUG"
    }
    runtime "Release"
    optimize "on"

filter "system:windows"
    buildoptions "/MT"
    staticruntime "on"
    defines 
    { 
        "BK_PLATFORM_WINDOWS" 
    }

filter "system:linux"
    staticruntime "on"
    defines 
    { 
        "BK_PLATFORM_LINUX" 
    }
