#include "Cuboid.hpp"
Cuboid::Cuboid(GLfloat height, GLfloat width, GLfloat depth) {

	height /= 2.0;
	width /= 2.0;
	depth /= 2.0;

	GLfloat vertexes[] ={
		-width, -height, -depth,
		-width, -height, depth,
		-width, height, -depth,
		-width, height, depth,
		width, -height, -depth,
		width, -height, depth,
		width, height, -depth,
		width, height, depth
	};

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
	SDL_assert(sizeof(vertexes) == 24*sizeof(GLfloat));
}

void Cuboid::draw() {

	 glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	 GLushort firstface[] = {0, 1, 2, 3, 6, 7, 4, 5, 1, 7, 3 , 6, 2, 4, 0, 1 };
	 GLuint indexBuffer;

	 glGenBuffers(1, &indexBuffer);
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstface), firstface, GL_STATIC_DRAW);
	 glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, 0);

	 SDL_assert(glGetError() == GL_NO_ERROR);
	 glDeleteBuffers(1, &indexBuffer);

}

Cuboid::~Cuboid() {
	 glDeleteBuffers(1, &vertexBuffer);
}
