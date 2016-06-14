#include "Cylinder.hpp"
#include <iostream>

GLuint Cylinder::vertexBuffer = 0;
GLuint Cylinder::normalBuffer = 0;
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
	glGenBuffers(1, &normalBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat), &vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,0,0);
	glBufferData(GL_ARRAY_BUFFER,vertices.size()*sizeof(GLfloat), &vertices[0],GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indecies.size()*sizeof(GLuint), &indecies[0], GL_STATIC_DRAW);
	vertexLength = indecies.size();
	GL_CATCH();
}
void Cylinder::Delete() {
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	glDeleteBuffers(1, &normalBuffer);
}
void Cylinder::DrawCylinder(){
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);

	glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
	glVertexAttribPointer(1, 3, GL_FLOAT,GL_FALSE,0,0);

	glDrawElements(GL_TRIANGLES,vertexLength,GL_UNSIGNED_SHORT,(void*)0);
};
Cylinder::Cylinder(const btVector3 &position, const btVector3 &size, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btCylinderShape *cylinder = new btCylinderShape(size/2.0);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0) {
		cylinder->calculateLocalInertia(mass,inert);
	}
	btMotionState *motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,cylinder,inert);
	body = new btRigidBody(RBCI);
}
Cylinder::~Cylinder() {
	delete body;
}
void Cylinder::draw() const {
	btVector3 half_size = ((btBoxShape*)body->getCollisionShape())->getHalfExtentsWithoutMargin();
	GLfloat matrix[16];
	btTransform bullet_transform;
	body->getMotionState()->getWorldTransform(bullet_transform);
	bullet_transform.getOpenGLMatrix(matrix);

	MatrixSender::SetModel(glm::scale(glm::make_mat4(matrix),
		glm::vec3(half_size.x(),half_size.y(),half_size.z())));
	MatrixSender::CalculateMVP();
	MatrixSender::SendMVP();
	MatrixSender::CalculateNormal();
	MatrixSender::SendNormal();
	DrawCylinder();
}
