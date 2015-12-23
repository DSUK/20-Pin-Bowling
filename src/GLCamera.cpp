#include "GLCamera.hpp"
GLCamera::GLCamera(glm::vec3 pos) {
	position = pos;
	yRotation = 0.0;
	xzRotation = 0.0;
	upVector = glm::vec3(0.0,1.0,0.0);
	lookVector = glm::vec3(0.0,0.0,-1.0);
	leftVector = glm::cross(upVector,lookVector);
}

glm::mat4 GLCamera::calculateViewMatrix() {
	return glm::lookAt(position,position + lookVector, upVector);
}

void GLCamera::setMatrixSenderViewMatrix() const {
	MatrixSender::SetView(glm::lookAt(position,position + lookVector, upVector));
}

void GLCamera::rotateXY(int x, int y,Uint32 time) {
	GLfloat ftime = static_cast<float>(time)/9600.0f;
	xzRotation += static_cast<float>(x)*ftime;
	yRotation -= static_cast<float>(y)*ftime;
	float sin_xz = sin(xzRotation);
	float cos_xz = cos(xzRotation);
	leftVector = glm::vec3(cos_xz,0,sin_xz);
	glm::vec3 temp_vec = glm::vec3(sin_xz,0,-cos_xz); //"face vector"
	upVector = glm::rotate(temp_vec,yRotation+90,leftVector);
	lookVector = glm::rotate(temp_vec,yRotation,leftVector);
}

void GLCamera::setFowardMove(float amount) {
	upMove = amount;
}

void GLCamera::setLeftMove(float amount) {
	leftMove = -amount;
}

void GLCamera::move(Uint32 time) {
	position += (lookVector*upMove + leftMove*leftVector)*static_cast<float>(time/16);
}

glm::vec3 GLCamera::getLook() const {
	return lookVector;
}

glm::vec3 GLCamera::getPos() const {
	return position;
}
