#include "PhysicsWorld.hpp"
PhysicsWorld::PhysicsWorld(GLuint model_matrix, GLuint colour_matrix): physicsDrawer(model_matrix, colour_matrix) {

	collisionConfig = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfig);
	breadphaseInterface = new btDbvtBroadphase();

	constraintSolver = new btSequentialImpulseConstraintSolver();
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, breadphaseInterface, constraintSolver, collisionConfig);
	dynamicsWorld->setGravity(btVector3(0.0, -9.8, 0.0));
}

PhysicsWorld::~PhysicsWorld() {

	for(unsigned int i = 0; i < bodies.size();++i) {
		dynamicsWorld->removeCollisionObject(bodies[i]->body);
		btMotionState *ms = bodies[i]->body->getMotionState();
		btCollisionShape *sp = bodies[i]->body->getCollisionShape();
		delete bodies[i];
		delete sp;
		delete ms;
	}
	delete dispatcher;
	delete collisionConfig;
	delete constraintSolver;
	delete breadphaseInterface;
	delete dynamicsWorld;
}

void PhysicsWorld::addBody(btRigidBody* bullet_body) {

	glm::mat3 colour;
	switch(bullet_body->getCollisionShape()->getShapeType())
	{
		case SPHERE_SHAPE_PROXYTYPE:
			colour = glm::mat3(0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
		break;
		case BOX_SHAPE_PROXYTYPE:
			colour = glm::mat3(1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
		break;
		case CYLINDER_SHAPE_PROXYTYPE:
			colour = glm::mat3(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
		default:
		break;

	}
	PhysicsDrawable *physics_body = new PhysicsDrawable(bullet_body, colour);
	bodies.push_back(physics_body);
	dynamicsWorld->addRigidBody(physics_body->body);
}

void PhysicsWorld::addBody(PhysicsDrawable* physics_body) {
	bodies.push_back(physics_body);
	dynamicsWorld->addRigidBody(physics_body->body);
}

void PhysicsWorld::incrementTime(GLfloat amount) {
	dynamicsWorld->stepSimulation(amount);
}

void PhysicsWorld::drawWorld() {

	for(GLuint i = 0; i < bodies.size(); ++i) {
		switch(bodies[i]->body->getCollisionShape()->getShapeType()) {

			case SPHERE_SHAPE_PROXYTYPE:
				physicsDrawer.drawBall(bodies[i]);
			break;
			case BOX_SHAPE_PROXYTYPE:
				physicsDrawer.drawCuboid(bodies[i]);
			break;
			case CYLINDER_SHAPE_PROXYTYPE:
				physicsDrawer.drawCylinder(bodies[i]);
			break;
			default:

			break;
		}
	}
}
