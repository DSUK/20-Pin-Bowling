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
	glDeleteBuffers(1, &indexBuffer);
}
void Cuboid::draw() {
}
void Cuboid::DrawCuboid(){
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,(void*)0);
}
