#include "PhysicsDrawable.hpp"

PhysicsDrawable::~PhysicsDrawable() {}
btRigidBody* PhysicsDrawable::getBody() const {
	return body;
}
