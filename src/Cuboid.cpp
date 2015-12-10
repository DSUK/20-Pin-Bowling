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
	GL_CATCH();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GL_CATCH();
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
	GL_CATCH();
}

void Cuboid::draw() {

	 GL_CATCH();
	 glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	 GL_CATCH();
	 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	 GL_CATCH();
	 GLushort firstface[] = {0, 1, 2, 3, 6, 7, 4, 5, 1, 7, 3 , 6, 2, 4, 0, 1 };
	 GL_CATCH();
	 GLuint indexBuffer;
	 GL_CATCH();

	 glGenBuffers(1, &indexBuffer);
	 GL_CATCH();
	 glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	 GL_CATCH();
	 glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstface), firstface, GL_STATIC_DRAW);
	 GL_CATCH();
	 //glDrawElements(GL_TRIANGLE_STRIP, 16, GL_UNSIGNED_SHORT, 0);
	 //force work, here
		GLuint tempVBO;
		GLfloat tempTriangle[] = {1.0,1.0,0.0, -1.0,1.0,0.0, -1.0, -1.0, 0.0};
		glGenBuffers(1,&tempVBO);
		glBindBuffer(GL_ARRAY_BUFFER,tempVBO);
		glBufferData(GL_ARRAY_BUFFER, 9*sizeof(GL_FLOAT), &tempTriangle[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
		glDrawArrays(GL_TRIANGLES, 0,3);
	 //end force work	 

	 GL_CATCH();
	 glDeleteBuffers(1, &indexBuffer);

}

Cuboid::~Cuboid() {
	 glDeleteBuffers(1, &vertexBuffer);
}
