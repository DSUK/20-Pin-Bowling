#pragma once

#include "PhysicsDrawable.hpp"
#include <vector>
class Ball : public PhysicsDrawable {
	static GLuint vertexBuffer, normalBuffer;
	static GLuint trianglePointCount;
	static void CalculateRecursivePoints(std::vector<GLfloat> &verts,glm::vec3 first, glm::vec3 second,
		glm::vec3 third, GLuint level);

	public:
	static void DrawBall();
	static void Init();
	static void Delete();
	void draw() const;
	Ball(const btVector3 &position, GLfloat radius, GLfloat mass, const btVector3 &velocity = btVector3(0.0,0.0,0.0));
	~Ball();
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
