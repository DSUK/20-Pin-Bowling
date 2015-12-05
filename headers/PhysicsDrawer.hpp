#ifndef PHYS_DRAWER_HPP
#define PHYS_DRAWER_HPP
#define GLM_FORCE_RADIANS
#ifndef GL_GLEXT_PROTOTYPES
#define GL_GLEXT_PROTOTYPES 1
#endif
#include "Ball.hpp"
#include "PhysicsDrawable.hpp"
#include "Cuboid.hpp"
#include "Cylinder.hpp"
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <bullet/btBulletDynamicsCommon.h>
class PhysicsDrawer {
	Ball ball;
	Cuboid rectangle;
	Cylinder cylinder;
	GLuint modelMatrix;
	GLuint colourMatrix;
	public:
	PhysicsDrawer(GLuint modelMatrix,GLuint colourMatrix);
	~PhysicsDrawer();
	void drawBall(btRigidBody* ball);
	void drawCuboid(btRigidBody* cuboid);
	void drawBall(PhysicsDrawable* ball);
	void drawCuboid(PhysicsDrawable* cuboid);
	void drawCylinder(PhysicsDrawable* cylinder);
};
#endif
