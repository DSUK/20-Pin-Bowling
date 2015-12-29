#pragma once

#include "PhysicsDrawable.hpp"
class Cuboid : public PhysicsDrawable {
	static GLuint vertexBuffer;
	static GLuint indexBuffer;
	public:
	static void DrawCuboid();
	static void Init();
	static void Delete();
	void draw();
	Cuboid(btVector3 position, btVector3 size, GLfloat mass);
	~Cuboid();
};
/*
#define GL3_PROTOTYPES 1
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "debug.hpp"
class Cuboid {

	public:
		Cuboid(GLfloat height, GLfloat width, GLfloat depth);
		void draw();
		~Cuboid();
	private:
	GLuint vertexBuffer;
};
*/
