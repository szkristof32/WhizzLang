workspace "WhizzLang"
	configurations { "Debug", "Release" }
	startproject "wzc"
	conformancemode "on"

	language "C++"
	cppdialect "C++11"
	architecture "x86_64"
	staticruntime "off"
	multiprocessorcompile "on"

	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"NOMINMAX",
		"MEMTRACE",
	}

	filter "action:vs*"
		buildoptions { "/utf-8" }

	filter "configurations:Debug"
		optimize "off"
		symbols "on"
		defines "WL_DEBUG"

	filter "configurations:Release"
		optimize "full"
		symbols "default"
		defines "WZ_RELEASE"

	filter "system:windows"
		systemversion "latest"
		defines "WL_PLATFORM_WINDOWS"

	filter "system:linux"
		defines "WL_PLATFORM_LINUX"

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "vendor/fmt"

project "wzstd"
	kind "StaticLib"
	location "wzstd"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"wzstd/include/**.h",
		"wzstd/src/**.cpp",
	}

	includedirs
	{
		"wzstd/include"
	}

project "WhizzLang"
	kind "StaticLib"
	location "WhizzLang"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "WhizzLang/src/pch.cpp"

	files
	{
		"WhizzLang/src/**.h",
		"WhizzLang/src/**.cpp",

		"vendor/memtrace_gtest/**.h",
		"vendor/memtrace_gtest/**.cpp",
	}

	includedirs
	{
		"WhizzLang/src",

		"wzstd/include",
		"vendor/fmt/include",
		"vendor/memtrace_gtest"
	}

	links
	{
		"wzstd",
		"fmt"
	}

	filter "files:vendor/memtrace_gtest/**"
		enablepch "off"

project "wzc"
	kind "ConsoleApp"
	location "wzc"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "pch.h"
	pchsource "wzc/src/pch.cpp"

	files
	{
		"wzc/src/**.h",
		"wzc/src/**.cpp",
	}

	includedirs
	{
		"wzc/src",
		"WhizzLang/src",

		"wzstd/include",
		"vendor/fmt/include",
		"vendor/memtrace_gtest"
	}

	links
	{
		"wzstd",
		"WhizzLang",
		"fmt"
	}

project "Tests"
	kind "ConsoleApp"
	location "Tests"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"Tests/src/**.h",
		"Tests/src/**.cpp",
	}

	includedirs
	{
		"Tests/src",
		"WhizzLang/src",

		"wzstd/include",
		"vendor/fmt/include",
		"vendor/memtrace_gtest"
	}

	links
	{
		"wzstd",
		"WhizzLang",
		"fmt"
	}
