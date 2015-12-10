#pragma once

#define	GL3_PROTOTYPES 1
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include "debug.hpp"
class Cylinder {
	public:
		Cylinder(GLfloat radius,GLfloat height, GLuint slices);
		void draw();
		~Cylinder();
	private:
		GLuint vertexBuffer;
		GLuint topVBOid;
		GLuint bottomVBOid;
		GLuint wallVBOid;
		GLuint sliceCount;
};
