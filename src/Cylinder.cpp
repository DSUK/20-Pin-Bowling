#include "Cylinder.hpp"
Cylinder::Cylinder(GLfloat radius, GLfloat height, GLuint slices) {
	sliceCount = slices;
	SDL_assert(slices >= 2);
	GLfloat angle_delta = 2*M_PI / static_cast<GLfloat>(slices);
	GLuint end = slices*6 + 6;

	GLfloat *vertexes = new GLfloat[end];
	GLfloat angle = 0.0f;
	height /= 2.0;

	//first top centre index
	vertexes[0] = 0.0f;
	vertexes[1] = height;
	vertexes[2] = 0.0f;

	//first bottom centre index
	vertexes[3] = 0.0f;
	vertexes[4] = -height;
	vertexes[5] = 0.0f;

	for(GLuint i = 1; i < slices + 1; ++i) {
		GLfloat sine = radius*sin(angle);
		GLfloat cose = radius*cos(angle);

		//top = even index
		vertexes[6*i    ] = cose;
		vertexes[6*i + 1] = height;
		vertexes[6*i + 2] = sine;

		//bottom = odd index
		vertexes[6*i + 3] = cose;
		vertexes[6*i + 4] = -height;
		vertexes[6*i + 5] = sine;
		angle += angle_delta;
	}

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, end*sizeof(GLfloat), vertexes, GL_STATIC_DRAW);

	//indexes for wall
	GLushort *indexes = new GLushort[2*slices + 2];
	for(GLushort i = 0; i < 2*slices; ++i) {
		indexes[i] = i + 2;
	}

	indexes[2*slices] = 2;
	indexes[2*slices + 1] = 3;

	glGenBuffers(1, &wallVBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallVBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (2*slices + 2)*sizeof(GLfloat), indexes, GL_STATIC_DRAW);

	//indexes for top => even numbers
	for(GLushort i = 0; i <= slices; ++i) {
		indexes[i] = i + i;
	}

	indexes[slices + 1] = 2;
	glGenBuffers(1, &topVBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topVBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (slices + 2)*sizeof(GLfloat), indexes, GL_STATIC_DRAW);


	//indexes for bottom => odd numbers
	for(GLushort i = 0; i < slices + 1; ++i)
	{
		indexes[i] = i + i + 1;
	}
	indexes[slices + 1] = 3;
	glGenBuffers(1, &bottomVBOid);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomVBOid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (slices + 2)*sizeof(GLfloat), indexes, GL_STATIC_DRAW);

	delete indexes;
	delete vertexes;
}
void Cylinder::draw()
{
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallVBOid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLE_STRIP, 2*sliceCount + 2, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topVBOid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLE_FAN, sliceCount + 2, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomVBOid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLE_FAN, sliceCount + 2, GL_UNSIGNED_SHORT, 0);
}
Cylinder::~Cylinder()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &topVBOid);
	glDeleteBuffers(1, &bottomVBOid);
	glDeleteBuffers(1, &wallVBOid);
}
