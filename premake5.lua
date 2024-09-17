workspace "Bakacrypt"
    architecture "x64"
    configurations { "Debug", "Release" }
    startproject "App"

    flags
    {
        "MultiProcessorCompile"
    }

    linkgroups "On"
outputdir = "%{cfg.system}-%{cfg.architecture}-%{cfg.buildcfg}"

include "dependencies.lua"

group "App"
    include "app"
group ""
    