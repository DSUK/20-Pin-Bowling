#pragma once

#include <glm/glm.hpp>
#include <bullet/btBulletDynamicsCommon.h>
struct PhysicsDrawable {
	btRigidBody* body;
	glm::mat3 colour;
	PhysicsDrawable(btRigidBody *_body, glm::mat3 _colour) {
		body = _body;
		colour = _colour;
	}

	~PhysicsDrawable() {
		delete body;
	}
};
