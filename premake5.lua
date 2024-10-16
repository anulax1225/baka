workspace "graphics"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "App"

    flags
    {
        "MultiProcessorCompile"
    }

    linkgroups "On"
    toolset "clang"
outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "dependencies.lua"

group "App"
    include "app"
group ""
    