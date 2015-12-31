#include "Cylinder.hpp"

GLuint Cylinder::vertexBuffer = 0;
GLuint Cylinder::indexBuffer = 0;
GLuint Cylinder::vertexLength = 0;
constexpr GLuint CYLINDER_SEGMENT_COUNT = 40;
constexpr GLuint DOUBLE_CYLINDER_SEGMENT_COUNT = 2*CYLINDER_SEGMENT_COUNT;
void Cylinder::Init() {
	std::vector<GLfloat> vertices;
	std::vector<GLushort> indecies;
	//top centre
	vertices.push_back(0.0f);
	vertices.push_back(1.0f);
	vertices.push_back(0.0f);
	//bottom centre
	vertices.push_back(0.0f);
	vertices.push_back(-1.0f);
	vertices.push_back(0.0f);

	for(GLuint i = 0; i < DOUBLE_CYLINDER_SEGMENT_COUNT; i += 2) {
		GLfloat sine = sin(((float)i)*M_PI/(float)CYLINDER_SEGMENT_COUNT);
		GLfloat cosine = cos(((float)i)*M_PI/(float)CYLINDER_SEGMENT_COUNT);
		vertices.push_back(sine);
		vertices.push_back(1.0f);
		vertices.push_back(cosine);

		vertices.push_back(sine);
		vertices.push_back(-1.0f);
		vertices.push_back(cosine);
	}
	//top
	for(GLuint i = 2; i < DOUBLE_CYLINDER_SEGMENT_COUNT;) {
		indecies.push_back(0);
		indecies.push_back(i);
		i += 2;
		indecies.push_back(i);
	}
	indecies.push_back(0);
	indecies.push_back(2);
	indecies.push_back(DOUBLE_CYLINDER_SEGMENT_COUNT);
	//bottom
	for(GLuint i = 3; i < DOUBLE_CYLINDER_SEGMENT_COUNT;) {
		indecies.push_back(1);
		indecies.push_back(i);
		i += 2;
		indecies.push_back(i);
	}
	indecies.push_back(1);
	indecies.push_back(3);
	indecies.push_back(DOUBLE_CYLINDER_SEGMENT_COUNT+1);
	//sides
	for(GLuint i = 2; i < DOUBLE_CYLINDER_SEGMENT_COUNT;) {
		indecies.push_back(i);
		++i;
		indecies.push_back(i);
		++i;
		indecies.push_back(i);
		--i;
	}
	indecies.push_back(DOUBLE_CYLINDER_SEGMENT_COUNT);
	indecies.push_back(DOUBLE_CYLINDER_SEGMENT_COUNT+1);
	indecies.push_back(3);
	indecies.push_back(DOUBLE_CYLINDER_SEGMENT_COUNT);
	indecies.push_back(3);
	indecies.push_back(2);


	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat), &vertices[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indecies.size()*sizeof(GLfloat), &indecies[0], GL_STATIC_DRAW);
	vertexLength = indecies.size();
}
void Cylinder::Delete() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}
void Cylinder::DrawCylinder(){
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glDrawElements(GL_TRIANGLES,vertexLength,GL_UNSIGNED_SHORT,(void*)0);
};
/*
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
	GL_CATCH();
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	GL_CATCH();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wallVBOid);
	GL_CATCH();
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	GL_CATCH();
	//--get somthing to draw
		//glUseProgram();
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
		glDrawArrays(GL_TRIANGLES,0,3);
	//--/get somthing to draw
	GL_CATCH();
	//glDrawElements(GL_TRIANGLE_STRIP, 2*sliceCount + 2, GL_UNSIGNED_SHORT, 0);
	GL_CATCH();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, topVBOid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLE_FAN, sliceCount + 2, GL_UNSIGNED_SHORT, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bottomVBOid);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawElements(GL_TRIANGLE_FAN, sliceCount + 2, GL_UNSIGNED_SHORT, 0);
	GL_CATCH();
}
Cylinder::~Cylinder()
{
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &topVBOid);
	glDeleteBuffers(1, &bottomVBOid);
	glDeleteBuffers(1, &wallVBOid);
}
*/
