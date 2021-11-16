project "Sandbox"
	toolset ("clang")

	targetdir ("%{wks.location}/bin/" .. outputdir .. "%{prj.name}")
	objdir ("%{wks.location}/bin/" .. outputdir .. "%{prj.name}/int")

	files {
		"src/**.h",
		"src/*.c",
		"src/**.hh",
		"src/**.cc"
	}

	includedirs {
		"include"
	}

	filter "configurations:Debug"
		kind "StaticLib"
		optimize "Debug"
		symbols "Full"

		defines {
			"_DEBUG",
			"LI_DEBUG",
			"LI_ASSERTIONS_ENABLED"
		}

	filter "configurations:Release"
		kind "StaticLib"
		optimize "On"
		
		defines {
			"LI_RELEASE"
		}

	filter "configurations:Distribution"
		kind "StaticLib"
		optimize "Speed"
		symbols "Off"

		defines {
			"LI_DISTRIBUTION"
		}

	filter "configurations:DebugDLL"
		kind "SharedLib"
		optimize "Debug"
		symbols "Full"

		defines {
			"_DEBUG",
			"LI_DLL",
			"LI_DEBUG",
			"LI_ASSERTIONS_ENABLED"
		}

	filter "configurations:ReleaseDLL"
		kind "SharedLib"
		optimize "On"
		
		defines {
			"LI_DLL",
			"LI_RELEASE"
		}

	filter "configurations:DistributionDLL"
		kind "SharedLib"
		optimize "Speed"
		symbols "Off"

		defines {
			"LI_DLL",
			"LI_DISTRIBUTION"
		}

	filter "system:windows"
		defines {
			"LI_PLATFORM_WINDOWS"
		}

	filter "system:osx"
		defines {
			"LI_PLATFORM_OSX"
		}

	filter "system:linux"
		defines {
			"LI_PLATFORM_LINUX"
		}

