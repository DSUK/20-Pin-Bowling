#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <SDL2/SDL_opengl.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "debug.hpp"
class PhysicsDrawable {
	public:
	btRigidBody* body;
	glm::mat3 colour;
	virtual void draw() = 0;
	virtual ~PhysicsDrawable() = 0;
};
