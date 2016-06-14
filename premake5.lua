workspace "20PinBowling"
	configurations { "Debug", "Release" }
	platforms { "x86", "x86_64" }

project "20PinBowling"
	kind "ConsoleApp"
	language "C++"
	files { "src/*.cpp" }
	includedirs { "headers", "/usr/include/bullet" }
	links { "SDL2test", "SDL2", "glew32", "opengl32", "BulletSoftBody", "BulletDynamics", "BulletCollision", "LinearMath" }
--TODO: windows ->  SDL2test glew32 opengl32
--TODO: linux -> SDL2_test GLEW GL pthread
--TODO: Bullet lib / app binary compatibility.

--TODO: create a directory with root modules for dependencies
configuration { "gmake" }
	buildoptions{ "-std=c++11", "-Wall", "-Werror" }
