#pragma once

#include "PhysicsDrawable.hpp"
class Cuboid : public PhysicsDrawable {
	static GLuint vertexBuffer;
	static GLuint indexBuffer;
	public:
	static void DrawCuboid();
	static void Init();
	static void Delete();
	void draw() const;
	Cuboid(btVector3 position, btVector3 size, GLfloat mass);
	~Cuboid();
};
