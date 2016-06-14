#include "Cuboid.hpp"
GLuint Cuboid::vertexBuffer = 0;
GLuint Cuboid::indexBuffer = 0;
GLuint Cuboid::normalBuffer = 0;

Cuboid::Cuboid(const btVector3 &position, const btVector3 &size, GLfloat mass) {
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
	glGenBuffers(1, &normalBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,0,0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexes), vertexes, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indecies), indecies, GL_STATIC_DRAW);
	GL_CATCH();

}
void Cuboid::Delete() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &normalBuffer);
	glDeleteBuffers(1, &indexBuffer);
}
void Cuboid::draw() const {
	btVector3 half_size = ((btBoxShape*)(body->getCollisionShape()))->getHalfExtentsWithoutMargin();
	GLfloat matrix[16];
	btTransform bullet_transform;
	body->getMotionState()->getWorldTransform(bullet_transform);
	bullet_transform.getOpenGLMatrix(matrix);
	MatrixSender::SetModel(glm::scale(glm::make_mat4(matrix),glm::vec3(half_size.x(),half_size.y(),half_size.z())));
	MatrixSender::CalculateMVP();
	MatrixSender::SendMVP();
	MatrixSender::CalculateNormal();
	MatrixSender::SendNormal();
	DrawCuboid();
}
void Cuboid::DrawCuboid(){
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glBindBuffer(GL_ARRAY_BUFFER,  normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,0,0);
	glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_SHORT,(void*)0);
}
