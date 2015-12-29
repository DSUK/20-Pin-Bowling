#include "Cuboid.hpp"
GLuint Cuboid::vertexBuffer = 0;
GLuint Cuboid::indexBuffer = 0;

Cuboid::Cuboid(btVector3 position, btVector3 size, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btBoxShape *box = new btBoxShape(size/2.0);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0) {
		box->calculateLocalInertia(mass,inert);
	}
	btMotionState *motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,box,inert);
	body = new btRigidBody(RBCI);
}
Cuboid::~Cuboid() {
	delete body;
}
void Cuboid::Init() {
	GLfloat vertexes[] ={
		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f
	};
	GLushort indecies[] = {
		0,1,2,0,1,4,0,2,4,
		1,4,5,1,5,7,4,5,7,
		1,3,7,2,3,7,1,2,3,
		2,4,6,2,6,7,4,6,7
	};

	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
	GL_CATCH();

}
void Cuboid::Delete() {
	glDeleteBuffers(1, &vertexBuffer);
}
void Cuboid::draw() {
}
void Cuboid::DrawCuboid(){
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	//glDrawArrays(GL_TRIANGLES,0,6);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,(void*)0);
}
/*
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

	 GL_CATCH();
	 glDeleteBuffers(1, &indexBuffer);

}

Cuboid::~Cuboid() {
	 glDeleteBuffers(1, &vertexBuffer);
}
*/
