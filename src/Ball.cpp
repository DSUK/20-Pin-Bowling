#include "Ball.hpp"
GLuint Ball::vertexBuffer = 0;
GLuint Ball::trianglePointCount = 0;
constexpr GLuint BALL_RECUR_LEVEL = 5;
constexpr GLuint BALL_VECTOR_RESERVE = BALL_RECUR_LEVEL * 24*(1 << 2*BALL_RECUR_LEVEL);
void Ball::Init() {
	std::vector<GLfloat> vertices;
	vertices.reserve(BALL_VECTOR_RESERVE);
	trianglePointCount = 0;
	CalculateRecursivePoints(vertices,glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,1.0f), BALL_RECUR_LEVEL);
	CalculateRecursivePoints(vertices,glm::vec3(-1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(0.0f,0.0f,-1.0f), BALL_RECUR_LEVEL);
	glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(GLfloat), &vertices[0],GL_STATIC_DRAW);
	GL_CATCH();
}

void Ball::CalculateRecursivePoints(std::vector<GLfloat> &points,glm::vec3 a, glm::vec3 b, glm::vec3 c, GLuint level) {
	if(level != 0) {
		glm::vec3 ab = glm::normalize(a + b);
		glm::vec3 ac = glm::normalize(a + c);
		glm::vec3 bc = glm::normalize(b + c);
		CalculateRecursivePoints(points,a,ab,ac,level-1);
		CalculateRecursivePoints(points,b,ab,bc,level-1);
		CalculateRecursivePoints(points,c,ac,bc,level-1);
		CalculateRecursivePoints(points,ab,ac,bc,level-1);
	} else {
		points.push_back(a.x);
		points.push_back(a.y);
		points.push_back(a.z);
		points.push_back(b.x);
		points.push_back(b.y);
		points.push_back(b.z);
		points.push_back(c.x);
		points.push_back(c.y);
		points.push_back(c.z);
		Ball::trianglePointCount += 3;
	}
}
void Ball::DrawBall() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES,0,trianglePointCount);
};
void Ball::Delete() {
	glDeleteBuffers(1,&vertexBuffer);
}
void Ball::draw() const {
	GLfloat matrix[16];
	btTransform bullet_transform;
	body->getMotionState()->getWorldTransform(bullet_transform);
	bullet_transform.getOpenGLMatrix(matrix);
	MatrixSender::SetModel(glm::scale(glm::make_mat4(matrix),glm::vec3(((btSphereShape*)body->getCollisionShape())->getRadius())));
	MatrixSender::CalculateMVP();
	MatrixSender::SendMVP();
	DrawBall();
}
Ball::~Ball() {
	delete body;
}
Ball::Ball(btVector3 position, GLfloat radius, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btSphereShape *sphere = new btSphereShape(radius);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0) {
		sphere->calculateLocalInertia(mass,inert);
	}
	btMotionState* motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,sphere,inert);
	body = new btRigidBody(RBCI);
}
/*
Ball::Ball(GLfloat _radius, GLuint segment_resolution, GLuint segment_count) {
	segmentCount = segment_count;
	segmentResolution= segment_resolution;
	GLuint array_size = 6*segment_resolution+6;
	radius = _radius;

	vertexBuffers = new GLuint[segment_count];
	GLfloat *vert_data = new GLfloat[array_size];
	glGenBuffers(segmentCount, vertexBuffers);
	float following, leading, delta;

	following = 0.0f;
	leading = delta = 2.0f*M_PI/static_cast<float>(segmentCount);

	for(GLuint i = 0; i < segmentCount; ++i) {
		createSegment(following, leading, vert_data, array_size, i);
		following += delta;
		leading += delta;
	}

	delete[] vert_data;
}

Ball::~Ball() {
	glDeleteBuffers(segmentCount, vertexBuffers);
	delete[] vertexBuffers;
}

void Ball::createSegment(GLfloat angle_begin, GLfloat angle_end, GLfloat *vert_data, GLuint array_size, GLuint buffer_index) {

	for(GLuint i = 0; i <= segmentResolution; ++i) {
		GLfloat height = radius*cos(static_cast<float>(i)*M_PI/static_cast<float>(segmentResolution));
		GLfloat outwards = radius*sin(static_cast<float>(i)*M_PI/static_cast<float>(segmentResolution));

		vert_data[6*i    ] = sin(angle_begin)*outwards;
		vert_data[6*i + 1] = height;
		vert_data[6*i + 2] = cos(angle_begin)*outwards;
		vert_data[6*i + 3] = sin(angle_end)*outwards;
		vert_data[6*i + 4] = height;
		vert_data[6*i + 5] = cos(angle_end)*outwards;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[buffer_index]);
	glBufferData(GL_ARRAY_BUFFER, array_size*sizeof(*vert_data), vert_data, GL_STATIC_DRAW);
}

void Ball::draw() {
	for(GLuint i = 0; i < segmentCount; ++i) {
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffers[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,0,0);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*segmentResolution + 1);
	}

}
*/
