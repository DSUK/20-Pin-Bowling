#pragma once

#define GLM_FORCE_RADIANS
#include <cmath>
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/glm.hpp>
#include "MatrixSender.hpp"
#include <SDL2/SDL.h>
class GLCamera {
	float yRotation;
	float xzRotation;
	glm::vec3 position;
	glm::vec3 lookVector;
	glm::vec3 upVector;
	glm::vec3 leftVector;
	float leftMove;
	float upMove;

	public:
	GLCamera(glm::vec3 pos);
	glm::mat4 calculateViewMatrix();
	void rotateXY(int x, int y, Uint32 time);
	void setFowardMove(float amount);
	void setLeftMove(float amount);
	void move(Uint32 time);
	void setMatrixSenderViewMatrix() const;
	glm::vec3 getLook() const;
	glm::vec3 getPos() const;
};
