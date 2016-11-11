local SRC_DIR            = "../src/"
local ALL_SRC_DIR        = SRC_DIR .. "all/"
local ALL_EXTERN_DIR     = ALL_SRC_DIR .. "apt/extern/"
local WIN_SRC_DIR        = SRC_DIR .. "win/"
local WIN_EXTERN_DIR     = WIN_SRC_DIR .. "apt/extern/"
local TESTS_DIR          = "../tests/"
local TESTS_EXTERN_DIR   = TESTS_DIR .. "extern/"

filter { "configurations:debug" }
	defines { "APT_DEBUG" }
	targetsuffix "_debug"
	symbols "On"
	optimize "Off"
	
filter { "configurations:release" }
	defines { "APT_DEBUG" }
	symbols "Off"
	optimize "Full"

filter { "action:vs*" }
	defines { "_CRT_SECURE_NO_WARNINGS", "_SCL_SECURE_NO_WARNINGS" }
	characterset "MBCS" -- force Win32 API to use *A variants (i.e. can pass char* for strings)

workspace "ApplicationTools"
	location(_ACTION)
	configurations { "Debug", "Release" }
	platforms { "Win64" }
	flags { "C++11", "StaticRuntime" }
	filter { "platforms:Win64" }
		system "windows"
		architecture "x86_64"
	
	includedirs { ALL_SRC_DIR, ALL_EXTERN_DIR }
	filter { "platforms:Win*" }
		includedirs { WIN_SRC_DIR, WIN_EXTERN_DIR }
		
	project "ApplicationTools"
		kind "StaticLib"
		language "C++"
		targetdir "../lib"
		uuid "6ADD11F4-56D6-3046-7F08-16CB6B601052"
		
		files({ ALL_SRC_DIR .. "**", ALL_EXTERN_DIR .. "**" })
		removefiles({ ALL_EXTERN_DIR .. "**.h", ALL_EXTERN_DIR .. "glm/**", ALL_EXTERN_DIR .. "rapidjson/**" })
		filter { "platforms:Win*" }
			files({ WIN_SRC_DIR .. "**" })
			removefiles({ WIN_EXTERN_DIR .. "**.h" })
		
		
	project "ApplicationTools_Tests"
		kind "ConsoleApp"
		language "C++"
		targetdir "../bin"
		uuid "DC3DA4C6-C837-CD18-B1A4-63299D3D3385"
		
		includedirs { TESTS_DIR, TESTS_EXTERN_DIR }
		files({ TESTS_DIR .. "**", TESTS_EXTERN_DIR .. "**" })
			removefiles({ TESTS_EXTERN_DIR .. "**.h" })
			
		links { "ApplicationTools" }
		filter { "platforms:Win*" }
			links { "shlwapi" }