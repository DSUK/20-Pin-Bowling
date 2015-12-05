#ifndef CUBOID_HPP
#define CUBOID_HPP
#define GL3_PROTOTYPES 1
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
class Cuboid {

	public:
		Cuboid(GLfloat height, GLfloat width, GLfloat depth);
		void draw();
		~Cuboid();
	private:
	GLuint vertexBuffer;
};
#endif
