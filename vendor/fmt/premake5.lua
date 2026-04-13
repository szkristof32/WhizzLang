project "fmt"
	kind "StaticLib"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/fmt-c.cc",
		"src/format.cc",
		"src/os.cc"
	}

	includedirs
	{
		"include"
	}
