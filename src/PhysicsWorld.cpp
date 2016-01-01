#include "PhysicsWorld.hpp"
PhysicsWorld::PhysicsWorld() {

	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	breadphaseInterface = new btDbvtBroadphase();

	constraintSolver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, breadphaseInterface, constraintSolver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
}

PhysicsWorld::~PhysicsWorld() {

	for(auto &body : bodies) {
		dynamicsWorld->removeCollisionObject(body->getBody());
		btMotionState *ms = body->getBody()->getMotionState();
		btCollisionShape *sp = body->getBody()->getCollisionShape();
		delete body;
		delete sp;
		delete ms;
	}
	delete dispatcher;
	delete collisionConfig;
	delete constraintSolver;
	delete breadphaseInterface;
	delete dynamicsWorld;
}

void PhysicsWorld::addBody(PhysicsDrawable* physics_body) {
	bodies.push_back(physics_body);
	dynamicsWorld->addRigidBody(physics_body->getBody());
}

void PhysicsWorld::incrementTime(GLfloat amount) {
	dynamicsWorld->stepSimulation(amount);
}

void PhysicsWorld::drawWorld() {
	for(auto &body : bodies) {
		body->draw();
	}
}
