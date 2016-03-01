workspace "20PinBowling"
	configurations { "Debug", "Release" }

project "20PinBowling"
	kind "ConsoleApp"
	language "C++"
	files { "src/*.cpp" }
	includedirs { "headers", "/usr/include/bullet" }
	links { "SDL2_test", "SDL2", "GLEW", "GL", "pthread", "BulletSoftBody", "BulletDynamics", "BulletCollision", "LinearMath" }

--TODO: create a directory with root modules for dependencies
configuration { "gmake", "gcc", "clang" }
	buildoptions{ "-std=c++11", "-Wall", "-Werror" }
