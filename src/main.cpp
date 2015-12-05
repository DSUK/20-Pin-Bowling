#define GLM_FORCE_RADIANS
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#include <cmath>
#include <SDL2/SDL.h>
#include "GLSLLoader.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "GLCamera.hpp"
#include <SDL2/SDL_test_common.h>
#include <SDL2/SDL_opengl.h>
#include "Ball.hpp"
#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsWorld.hpp"
#include "Cuboid.hpp"
//#include "CylinderDrawer.hpp"
#define ERLOG(X,Y) \
		do {if((Y = X)) {\
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed " #X "on line %d : %s\n", __LINE__,SDL_GetError()); \
			exit(Y); \
		} }while(0)
const float kSqrt2 = 1.41421356237;
const float kSqrt3Over4 = 0.86602540378;
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
int triangleNum(int number) {
	return (number*(number+1))/2;
}

btVector3 findLeftChild(btVector3 node,btScalar radius) {
	return btVector3(node.x()+radius/2, node.y(),node.z() - radius/kSqrt2);
}

btVector3 findRightChild(btVector3 node,btScalar radius) {
	return btVector3(node.x()-radius/2, node.y(),node.z() - radius/kSqrt2);
}

btRigidBody* createSphere(btVector3 position,GLfloat radius, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btSphereShape *sphere = new btSphereShape(radius);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0)
	{
		sphere->calculateLocalInertia(mass,inert);
	}
	btMotionState* motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,sphere,inert);
	btRigidBody* body = new btRigidBody(RBCI);
	return body;
}
btRigidBody* createBox(btVector3 position, btVector3 size, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btBoxShape *box = new btBoxShape(size/2.0);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0)
	{
		box->calculateLocalInertia(mass,inert);
	}
	btMotionState *motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,box,inert);
	btRigidBody* body = new btRigidBody(RBCI);
	return body;
}

btRigidBody* createCylinder(btVector3 position, btVector3 size, GLfloat mass) {
	btTransform trans;
	trans.setIdentity();
	trans.setOrigin(position);
	btCylinderShape *cylinder = new btCylinderShape(size/2.0);
	btVector3 inert(0.0,0.0,0.0);
	if(mass != 0.0)
	{
		cylinder->calculateLocalInertia(mass,inert);
	}
	btMotionState *motion = new btDefaultMotionState(trans);
	btRigidBody::btRigidBodyConstructionInfo RBCI(mass,motion,cylinder,inert);
	btRigidBody* body = new btRigidBody(RBCI);
	return body;
}

void testcube() {
	GLuint vertexbuffer;
	GLfloat Height = 0.5f;
	GLfloat Width = 0.5f;
	GLfloat Depth = 0.5f;

	GLfloat vertexes[] =
	{-Width,-Height,-Depth,
	-Width,-Height,Depth,
	-Width,Height,-Depth,
	-Width,Height,Depth,
	Width,-Height,-Depth,
	Width,-Height,Depth,
	Width,Height,-Depth,
	Width,Height,Depth};
	glGenBuffers(1,&vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertexes),vertexes,GL_STATIC_DRAW);
	SDL_assert(sizeof(vertexes) == 24*sizeof(GLfloat));
	glBindBuffer(GL_ARRAY_BUFFER,vertexbuffer);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_LINE_STRIP,0,8);
	//glDrawElements(GL_POINTS,4,GL_UNSIGNED_SHORT,firstface);
	SDL_assert(glGetError() == GL_NO_ERROR);

}

void mainloop(SDL_Window *display) {

	GLCamera cam(glm::vec3(0,0,2));
	GLSLLoader shaders;
	shaders.loadFile(GL_VERTEX_SHADER,"./glsl/Vertex.vert");
	shaders.loadFile(GL_FRAGMENT_SHADER,"./glsl/Fragment.frag");
	shaders.compile(GL_VERTEX_SHADER);
	shaders.compile(GL_FRAGMENT_SHADER);
	shaders.attach(GL_VERTEX_SHADER);
	shaders.attach(GL_FRAGMENT_SHADER);
	shaders.link();
	glUseProgram(shaders.getProgramObject());
	SDL_Event test;

	glm::mat4 Perspective = glm::perspective(45.0f, 4.0f/3.0f,0.001f,1000.0f);
	glm::mat4 Viewpoint = glm::lookAt(glm::vec3(0,0,1),glm::vec3(0,0,0),glm::vec3(0,1,0));
	glm::mat4 Model = glm::mat4(1.0);
	GLuint PerspectiveID = glGetUniformLocation(shaders.getProgramObject(),"Perspective");
	GLuint ViewpointID = glGetUniformLocation(shaders.getProgramObject(),"Viewpoint");
	GLuint ModelMatrixID = glGetUniformLocation(shaders.getProgramObject(),"Model");
	glUniformMatrix4fv(PerspectiveID,1,GL_FALSE,&Perspective[0][0]);
	glUniformMatrix4fv(ViewpointID,1,GL_FALSE,&Viewpoint[0][0]);
	glUniformMatrix4fv(ModelMatrixID,1,GL_FALSE,&Model[0][0]);
	GLuint ColourID = glGetUniformLocation(shaders.getProgramObject(),"vert_colour");
	GLuint Lightpos = glGetUniformLocation(shaders.getProgramObject(),"vert_lightPos");
	GLuint Lightpos2 = glGetUniformLocation(shaders.getProgramObject(),"vert_lightPos2");
	GLuint LightCol = glGetUniformLocation(shaders.getProgramObject(),"vert_lightColour");
	/*GLuint NormalVal=*/ glGetUniformLocation(shaders.getProgramObject(),"vert_normal");
	PhysicsWorld phys(ModelMatrixID,ColourID);
	glUniform3f(Lightpos,1.0,3.0,0.0);
	glUniform3f(Lightpos2,1.0,3.0,-50.0);
	glUniformMatrix3fv(LightCol,1,GL_FALSE,&glm::mat3(1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0)[0][0]);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnableVertexAttribArray(0);
	bool enablemouse = false;
	bool cont = true;

	SDL_assert(glGetError() == GL_NO_ERROR);

	Uint32 LoopTime;
	PhysicsDrawable* floor;
	{
		btRigidBody* _floor = createBox(btVector3(0,-4,-90),btVector3(40,0.1,220),0.0);
		glm::mat3 floor_colour = glm::mat3(1.0,1.0,1.0, 1.0,1.0,1.0,1.0,1.0,1.0);
		PhysicsDrawable *temp = new PhysicsDrawable(_floor,floor_colour);
		floor = temp;
	}
	btRigidBody* cylinders[20];
	btVector3 start(0,0,-150);
	int e = 0; //end
	int i = 0; //index
	{
		int n = 4; //count on row
		while(n>0)
		{
			e+= n;
			btVector3 temp = start;
			while(i < e)
			{
				cylinders[i] = createCylinder(temp,btVector3(3,7,3),1.0);
				temp = findLeftChild(temp,3.0);
				++i;
			}
			start = findRightChild(start,3.0);
			--n;
		}
		SDL_assert(glGetError() == GL_NO_ERROR);
	}
	start = btVector3(0,7,-150-kSqrt3Over4*1.5);
	{
		int n = 3; //count on row
		while(n>0)
		{
			e+= n;
			btVector3 temp = start;
			while(i < e)
			{
				cylinders[i] = createCylinder(temp,btVector3(3,7,3),1.0);
				temp = findLeftChild(temp,3.0);
				++i;
			}
			start = findRightChild(start,3.0);
			--n;
		}
	}
	start = btVector3(0,14,-150-kSqrt3Over4*3);
	{
		int n = 2; //count on row
		while(n>0)
		{
			e+= n;
			btVector3 temp = start;
			while(i < e)
			{
				cylinders[i] = createCylinder(temp,btVector3(3,7,3),1.0);
				temp = findLeftChild(temp,3.0);
				++i;
			}
			start = findRightChild(start,3.0);
			--n;
		}
	}
	cylinders[i] = createCylinder(btVector3(0,21,-150-kSqrt3Over4*4.5),btVector3(3,7,3),1.0);

	for(int i = 0; i < 20; ++i)
	{
		phys.addBody(cylinders[i]);
	}


	btRigidBody* sphere = createSphere(btVector3(0,0,-20),1,1);
	//btRigidBody* boxx = createBox(btVector3(0,0,-10),btVector3(1,10,1),1);
	phys.addBody(sphere);
	phys.addBody(floor);
	while(cont)
	{
		LoopTime = SDL_GetTicks();
		phys.drawWorld();
		glUniformMatrix4fv(ModelMatrixID,1,GL_FALSE,&Model[0][0]);
		while(SDL_PollEvent(&test))
		{
			switch(test.type)
			{
				case SDL_KEYDOWN:
					switch(test.key.keysym.sym)
					{
						case SDLK_UP:
							cam.setFowardMove(0.05f);
						break;
						case SDLK_DOWN:
							cam.setFowardMove(-0.05);
						break;
						case SDLK_LEFT:
							cam.setLeftMove(0.05);
						break;
						case SDLK_RIGHT:
							cam.setLeftMove(-0.05);
						break;
						case SDLK_SPACE:
							SDL_ShowCursor(!SDL_ShowCursor(-1));
							enablemouse = !enablemouse;
						break;
						case SDLK_k:
							glDisableVertexAttribArray(0);
						default:
						break;
					}
				break;
				case SDL_KEYUP:
					switch(test.key.keysym.sym)
					{
						case SDLK_UP:
							cam.setFowardMove(0);
						break;
						case SDLK_DOWN:
							cam.setFowardMove(0);
						break;
						case SDLK_LEFT:
							cam.setLeftMove(0);
						break;
						case SDLK_RIGHT:
							cam.setLeftMove(0);
						break;
						case SDLK_ESCAPE:
						cont = false;
						break;
						default:
						break;
					}
				break;
				case SDL_MOUSEBUTTONDOWN:
					switch(test.button.button)
					{
						case SDL_BUTTON_LEFT:
							glm::vec3 thrower = cam.getPos();
							btRigidBody *a_sphere = createSphere(btVector3(thrower.x,thrower.y,thrower.z),1,10);
							thrower = cam.getLook();
							a_sphere->setLinearVelocity(btVector3(40.0*thrower.x,40.0*thrower.y,40.0*thrower.z));
							phys.addBody(a_sphere);
						break;
					}
				break;
				case SDL_QUIT:
					cont = false;
				break;
				default:
				break;
			}
		}
		if(enablemouse)
		{
			int centreX = kWindowWidth/2;
			int centreY = kWindowHeight/2;
			int mX,mY;
			mX = mY =0;
			SDL_GetMouseState(&mX,&mY);
			cam.rotateXY(mX-centreX,mY-centreY);
			SDL_WarpMouseInWindow(display,centreX,centreY);
		}
		cam.move();
		SDL_GL_SwapWindow(display);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		Viewpoint = cam.calculateViewMatrix();
		glUniformMatrix4fv(ViewpointID,1,GL_FALSE,&Viewpoint[0][0]);
		SDL_Delay(10);
		LoopTime = SDL_GetTicks() - LoopTime;
		phys.incrementTime(static_cast<float>(LoopTime)/1000.0f);
	}

	SDL_Quit();
}

int main() {
	SDL_Window *display;
	//static SDL_GLContext context;
	int value;
	int status;
	ERLOG(SDL_Init(SDL_INIT_EVERYTHING),status);
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	display = SDL_CreateWindow("20 Pin Bowling",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,kWindowWidth,kWindowHeight,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	ERLOG(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4),status);
	ERLOG(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0),status);
	ERLOG(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1),status);
	ERLOG(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24),status);
	ERLOG(SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value),status);
	//context =
	SDL_GL_CreateContext(display);
	glEnable(GL_DEPTH_TEST);
	mainloop(display);
	return 0;
}
