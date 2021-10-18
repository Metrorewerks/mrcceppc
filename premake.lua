newoption {
	trigger = "v10",
	description = "Allows premake to make a project for 1.0",
}

newoption {
	trigger = "v27",
	description = "Allows premake to make a project for 2.7",
}
workspace "mrcceppc"
	location "./build/"

	targetdir "%{wks.location}/bin/%{cfg.buildcfg}/"
	objdir "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}/"
	buildlog "%{wks.location}/obj/%{cfg.buildcfg}/%{prj.name}.log"

	largeaddressaware "on"
	editandcontinue "off"
	staticruntime "on"

	systemversion "latest"
	characterset "unicode"
	architecture "x86"
	warnings "extra"

	includedirs {
		"./src/",
	}
	
	buildoptions {
		"/Zm200",
		"/utf-8",
		"/std:c++17",
		"/bigobj",
	}

	flags {
		"noincrementallink",
		"no64bitchecks",
		"shadowedvariables",
		"undefinedidentifiers",
		"multiprocessorcompile",
	}

	defines {
		"NOMINMAX",
		"WIN32_LEAN_AND_MEAN",
		"_CRT_SECURE_NO_WARNINGS",
		"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS",
	}

	platforms {
		"x86",
	}

	configurations {
		"Release",
		"Debug",
	}

	configuration "Release"
		defines "NDEBUG"
		optimize "debug"
		runtime "debug"
		symbols "on"

	configuration "Debug"
		defines "DEBUG"
		optimize "debug"
		runtime "debug"
		symbols "on"

if _OPTIONS["v10"] then
	project "1.0"
		targetname "mrcceppc"
		language "c++"
		kind "consoleapp"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "src/1.0/stdafx.cpp"
		forceincludes "stdafx.hpp"

		postbuildcommands {
			"copy /y \"$(TargetPath)\" \"../ref/1.0/\""
		}

		files {
			"./src/1.0/**",
			"./src/loader/**",
			"./src/memory/**",
		}

		includedirs {
			"./src/1.0/",
			"./src/loader/",
			"./src/memory/",
		}
		
		--Required for manual mapping
		linkoptions{
			"/NXCOMPAT:NO",
			"/IGNORE:4254",
			"/DYNAMICBASE:NO",
			"/SAFESEH:NO",
			"/LARGEADDRESSAWARE",
			"/LAST:.main",
		}
end

if _OPTIONS["v27"] then
	project "2.7"
		targetname "mrcceppc"
		language "c++"
		kind "consoleapp"
		warnings "off"

		pchheader "stdafx.hpp"
		pchsource "src/2.7/stdafx.cpp"
		forceincludes "stdafx.hpp"

		postbuildcommands {
			"copy /y \"$(TargetPath)\" \"../ref/2.7/\""
		}

		files {
			"./src/2.7/**",
			"./src/loader/**",
			"./src/memory/**",
		}

		includedirs {
			"./src/2.7/",
			"./src/loader/",
			"./src/memory/",
		}
		
		--Required for manual mapping
		linkoptions{
			"/NXCOMPAT:NO",
			"/IGNORE:4254",
			"/DYNAMICBASE:NO",
			"/SAFESEH:NO",
			"/LARGEADDRESSAWARE",
			"/LAST:.main",
		}
end