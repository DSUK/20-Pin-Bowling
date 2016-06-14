#pragma once

#include "PhysicsDrawable.hpp"
class Cuboid : public PhysicsDrawable {
	static GLuint vertexBuffer;
	static GLuint indexBuffer;
	static GLuint normalBuffer;
	public:
	static void DrawCuboid();
	static void Init();
	static void Delete();
	void draw() const;
	Cuboid(const btVector3 &position, const btVector3 &size, GLfloat mass);
	~Cuboid();
};
