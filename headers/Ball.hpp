#pragma once

#include "PhysicsDrawable.hpp"
class Ball : public PhysicsDrawable {
	static void Init();
	static void DrawBall();
	void draw();
};

/*
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <glm/glm.hpp>
#include <cmath>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
class Ball {
	public:
	Ball(GLfloat radius, GLuint segment_resolution, GLuint segment_count);
	void draw();
	~Ball();

	protected:
	private:
	void createSegment(GLfloat angle_begin, GLfloat angle_end, GLfloat *vert_data, GLuint array_size, GLuint buffer_index);
	GLuint *vertexBuffers;
	GLuint segmentResolution;
	GLfloat radius;
	GLuint segmentCount;

};
*/
