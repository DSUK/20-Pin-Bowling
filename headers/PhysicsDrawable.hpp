#pragma once

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
//scale
#include <glm/gtc/matrix_transform.hpp>
//make_mat4
#include <glm/gtc/type_ptr.hpp>
#include <SDL2/SDL_opengl.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "debug.hpp"
#include "MatrixSender.hpp"
class PhysicsDrawable {
	protected:
	btRigidBody* body;
	glm::vec3 ambient_colour;
	public:
	virtual void draw() const = 0;
	virtual ~PhysicsDrawable() = 0;
	btRigidBody* getBody() const;
};
