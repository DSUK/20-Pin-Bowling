#include "PhysicsDrawer.hpp"
PhysicsDrawer::PhysicsDrawer(GLuint model_matrix, GLuint colour_matrix): ball(1.0f, 20, 20), rectangle(2.0, 2.0, 2.0), cylinder(1.0f, 2.0f, 20) {
	modelMatrix = model_matrix;
	colourMatrix = colour_matrix;
}

void PhysicsDrawer::drawBall(btRigidBody* ball_body) {

	SDL_assert(ball_body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE);
	glUniformMatrix3fv(colourMatrix, 1, GL_FALSE, &glm::mat3(0.0, 1.0, 1.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)[0][0]);

	glm::mat4 transfrom_matrix = glm::scale(glm::mat4(1.0), glm::vec3(((btSphereShape*)ball_body ->getCollisionShape())->getRadius()));

	float gl_matrix[16];
	glm::mat4 bullet_matrix;
	btTransform bullet_transform;
	ball_body->getMotionState()->getWorldTransform(bullet_transform);

	bullet_transform.getOpenGLMatrix(gl_matrix);
	bullet_matrix = glm::make_mat4(gl_matrix);
	transfrom_matrix = bullet_matrix*transfrom_matrix;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &transfrom_matrix[0][0]);

	ball.draw();
}

void PhysicsDrawer::drawBall(PhysicsDrawable* ball_body) {

	SDL_assert(ball_body->body->getCollisionShape()->getShapeType() == SPHERE_SHAPE_PROXYTYPE);
	glUniformMatrix3fv(colourMatrix, 1, GL_FALSE, &(ball_body->colour)[0][0]);

	glm::mat4 transformation_matrix = glm::scale(glm::mat4(1.0), glm::vec3(((btSphereShape*)ball_body->body->getCollisionShape())->getRadius()));

	float gl_matrix[16];
	glm::mat4 bullet_matrix;
	btTransform bullet_transformation;
	ball_body->body->getMotionState()->getWorldTransform(bullet_transformation);

	bullet_transformation.getOpenGLMatrix(gl_matrix);
	bullet_matrix = glm::make_mat4(gl_matrix);
	transformation_matrix = bullet_matrix*transformation_matrix;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &transformation_matrix[0][0]);

	ball.draw();
}

void PhysicsDrawer::drawCuboid(PhysicsDrawable* cuboid) {

	SDL_assert(cuboid->body->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE);
	glUniformMatrix3fv(colourMatrix, 1, GL_FALSE, &(cuboid->colour)[0][0]);

	btVector3 half_size = ((btBoxShape*)(cuboid->body)->getCollisionShape())->getHalfExtentsWithoutMargin();
	glm::mat4 transformation_matrix = glm::scale(glm::mat4(1.0), glm::vec3(half_size.x(), half_size.y(), half_size.z()));
	float gl_matrix[16];
	glm::mat4 bullet_matrix;

	btTransform bullet_transformation;
	cuboid->body->getMotionState()->getWorldTransform(bullet_transformation);
	bullet_transformation.getOpenGLMatrix(gl_matrix);
	bullet_matrix = glm::make_mat4(gl_matrix);

	transformation_matrix = bullet_matrix*transformation_matrix;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &transformation_matrix[0][0]);
	rectangle.draw();
}

void PhysicsDrawer::drawCuboid(btRigidBody* cuboid) {

	SDL_assert(cuboid->getCollisionShape()->getShapeType() == BOX_SHAPE_PROXYTYPE);
	glUniformMatrix3fv(colourMatrix, 1, GL_FALSE, &glm::mat3(1.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0)[0][0]);

	btVector3 half_size = ((btBoxShape*)cuboid->getCollisionShape())->getHalfExtentsWithoutMargin();
	glm::mat4 transformation_matrix = glm::scale(glm::mat4(1.0), glm::vec3(half_size.x(), half_size.y(), half_size.z()));
	float gl_matrix[16];
	glm::mat4 bullet_matrix;

	btTransform bullet_transformation;
	cuboid->getMotionState()->getWorldTransform(bullet_transformation);
	bullet_transformation.getOpenGLMatrix(gl_matrix);
	bullet_matrix = glm::make_mat4(gl_matrix);

	transformation_matrix = bullet_matrix*transformation_matrix;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &transformation_matrix[0][0]);
	rectangle.draw();
}

void PhysicsDrawer::drawCylinder(PhysicsDrawable* cylinder) {

	SDL_assert(cylinder->body->getCollisionShape()->getShapeType() == CYLINDER_SHAPE_PROXYTYPE);
	glUniformMatrix3fv(colourMatrix, 1, GL_FALSE, &(cylinder->colour)[0][0]);

	btVector3 half_size = ((btBoxShape*)(cylinder->body)->getCollisionShape())->getHalfExtentsWithoutMargin();
	glm::mat4 bullet_transformation = glm::scale(glm::mat4(1.0), glm::vec3(half_size.x(), half_size.y(), half_size.z()));
	float gl_matrix[16];
	glm::mat4 bullet_matrix;

	btTransform bullet_transform;
	cylinder->body->getMotionState()->getWorldTransform(bullet_transform);
	bullet_transform.getOpenGLMatrix(gl_matrix);
	bullet_matrix = glm::make_mat4(gl_matrix);

	bullet_transformation = bullet_matrix*bullet_transformation;
	glUniformMatrix4fv(modelMatrix, 1, GL_FALSE, &bullet_transformation[0][0]);
	this->cylinder.draw();
}

PhysicsDrawer::~PhysicsDrawer()
{}
