#pragma once

#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsDrawable.hpp"
#include "debug.hpp"
class PhysicsWorld {

	std::vector<PhysicsDrawable*> bodies;
	btDynamicsWorld* dynamicsWorld;
	btDispatcher* dispatcher;
	btCollisionConfiguration* collisionConfig;
	btBroadphaseInterface* breadphaseInterface;
	btConstraintSolver* constraintSolver;

	public:
	PhysicsWorld();
	~PhysicsWorld();
	void addBody(btRigidBody* bullet_body);
	void addBody(PhysicsDrawable* physics_body);
	void deleteBody();
	void incrementTime(GLfloat amount);
	void drawWorld();
};
