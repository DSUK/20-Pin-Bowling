#include "Ball.hpp"

Ball::Ball(GLfloat _radius, GLuint segment_resolution, GLuint segment_count) {
	segmentCount = segment_count;
	segmentResolution= segment_resolution;
	GLuint array_size = 6*segment_resolution+6;
	radius = _radius;

	vertexBuffers = new GLuint[segment_count];
	GLfloat *vert_data = new GLfloat[array_size];
	glGenBuffers(segmentCount, vertexBuffers);
	float following, leading, delta;

	following = 0.0f;
	leading = delta = 2.0f*M_PI/static_cast<float>(segmentCount);

	for(GLuint i = 0; i < segmentCount; ++i) {
		createSegment(following, leading, vert_data, array_size, i);
		following += delta;
		leading += delta;
	}

	delete[] vert_data;
}

Ball::~Ball() {
	glDeleteBuffers(segmentCount, vertexBuffers);
	delete[] vertexBuffers;
}

void Ball::createSegment(GLfloat angle_begin, GLfloat angle_end, GLfloat *vert_data, GLuint array_size, GLuint buffer_index) {

	for(GLuint i = 0; i <= segmentResolution; ++i) {
		GLfloat height = radius*cos(static_cast<float>(i)*M_PI/static_cast<float>(segmentResolution));
		GLfloat outwards = radius*sin(static_cast<float>(i)*M_PI/static_cast<float>(segmentResolution));

		vert_data[6*i    ] = sin(angle_begin)*outwards;
		vert_data[6*i + 1] = height;
		vert_data[6*i + 2] = cos(angle_begin)*outwards;
		vert_data[6*i + 3] = sin(angle_end)*outwards;
		vert_data[6*i + 4] = height;
		vert_data[6*i + 5] = cos(angle_end)*outwards;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[buffer_index]);
	glBufferData(GL_ARRAY_BUFFER, array_size*sizeof(*vert_data), vert_data, GL_STATIC_DRAW);
}

void Ball::draw() {
	for(GLuint i = 0; i < segmentCount; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*segmentResolution + 1);
	}

}
