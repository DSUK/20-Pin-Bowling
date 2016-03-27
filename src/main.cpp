#define GLM_FORCE_RADIANS
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#include <GL/glew.h>
//#include <cmath>
#include <SDL2/SDL.h>
#include "GLSLLoader.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "GLCamera.hpp"
#include <SDL2/SDL_test_common.h>
#include <SDL2/SDL_opengl.h>
#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsWorld.hpp"
#include "Ball.hpp"
#include "Cuboid.hpp"
#include "Cylinder.hpp"
#include "debug.hpp"
#include "MatrixSender.hpp"
#include <glm/gtc/type_ptr.hpp>

const float kSqrt2 = 1.41421356237;
//const float kSqrt3Over4 = 0.86602540378;
const int kWindowWidth = 1024;
const int kWindowHeight = 768;
const int kWindowCentreX = kWindowWidth/2;
const int kWindowCentreY = kWindowHeight/2;
int triangleNum(int number) {
	return (number*(number+1)) >> 1;
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

void main_loop(SDL_Window *display) {
	GLCamera camera(glm::vec3(0,0,2));
	GLSLLoader shaders;
	bool enablemouse = false;
	shaders.loadFile(GL_VERTEX_SHADER,"./glsl/test.vert");
	shaders.loadFile(GL_FRAGMENT_SHADER,"./glsl/test.frag");
	shaders.compile(GL_VERTEX_SHADER);
	shaders.compile(GL_FRAGMENT_SHADER);
	shaders.attach(GL_VERTEX_SHADER);
	shaders.attach(GL_FRAGMENT_SHADER);
	shaders.bindAttribute(0, "in_vertexpos");
	shaders.bindAttribute(1, "in_normal");
	shaders.link();
	shaders.useProgram();
	GL_CATCH();
	MatrixSender::Init(shaders.uniformLocation("MVP"), shaders.uniformLocation("NormalTransform"));
	MatrixSender::SetProjection(glm::perspective(45.0f, 4.0f/3.0f,0.001f,1000.0f));
	MatrixSender::SetModel(glm::mat4(1.0));
	MatrixSender::SetView(glm::lookAt(glm::vec3(1,1,1),glm::vec3(0,0,0),glm::vec3(0,1,0)));
	MatrixSender::CalculateMVP();
	MatrixSender::SendMVP();
	GLfloat temp[16];
	glGetUniformfv(shaders.getProgramObject(),glGetUniformLocation(shaders.getProgramObject(),"MVP"),temp);
	SDL_Event event;
	bool cont = true;
	GLuint VertexArrayId;
	glGenVertexArrays(1,&VertexArrayId);
	glBindVertexArray(VertexArrayId);

	Uint32 time = SDL_GetTicks();
	Uint32 loop_time = 0;
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	Cuboid::Init();
	atexit(Cuboid::Delete);
	Ball::Init();
	atexit(Ball::Delete);
	Cylinder::Init();
	atexit(Cylinder::Delete);
	PhysicsWorld world;
	world.addBody(new Cuboid(btVector3(0.0f,3.0f,-2.0f),btVector3(1.0f,1.0f,1.0f),1.0f));
	world.addBody(new Cuboid(btVector3(0.0f,-0.4f,-2.0f),btVector3(20.0f,0.1f,20.0f),0.0f));
	world.addBody(new Ball(btVector3(0.0f,2.0f,-2.01f),0.5f,1.0f));
	do {
		GL_CATCH();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
		world.drawWorld();
		world.incrementTime(0.0001f*loop_time);

		while(SDL_PollEvent(&event))
		{
			switch(event.type)
			{
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_w:
							camera.setFowardMove(0.05f);
						break;
						case SDLK_DOWN:
						case SDLK_s:
							camera.setFowardMove(-0.05f);
						break;
						case SDLK_LEFT:
						case SDLK_a:
							camera.setLeftMove(0.05f);
						break;
						case SDLK_RIGHT:
						case SDLK_d:
							camera.setLeftMove(-0.05f);
						break;
						case SDLK_SPACE:
							SDL_ShowCursor(!SDL_ShowCursor(-1));
							enablemouse = !enablemouse;
						break;
						default:
						break;
					}
				break;
				case SDL_KEYUP:
					switch(event.key.keysym.sym)
					{
						case SDLK_UP:
						case SDLK_w:
							camera.setFowardMove(0.0f);
						break;
						case SDLK_DOWN:
						case SDLK_s:
							camera.setFowardMove(0.0f);
						break;
						case SDLK_LEFT:
						case SDLK_a:
							camera.setLeftMove(0.0f);
						break;
						case SDLK_RIGHT:
						case SDLK_d:
							camera.setLeftMove(0.0f);
						break;
						case SDLK_ESCAPE:
							cont = false;
						break;
						default:
						break;
					}
				break;
				case SDL_MOUSEBUTTONDOWN:
					switch(event.button.button)
					{
						case SDL_BUTTON_LEFT:
							glm::vec3 look = 40.0f*camera.getLook();
							glm::vec3 pos = camera.getPos();
							world.addBody(new Ball(
								btVector3(pos.x,pos.y,pos.z),0.5f,1.0f,
								btVector3(look.x,look.y,look.z)));
						break;
					}
				break;
				case SDL_QUIT:
					cont = false;
				break;
				default:
				break;
			}
			if(enablemouse) {
				int mX,mY;
				mX = mY = 0;
				SDL_GetMouseState(&mX,&mY);
				camera.rotateXY(mX-kWindowCentreX,mY-kWindowCentreY,loop_time);
				SDL_WarpMouseInWindow(display,kWindowCentreX,kWindowCentreY);

			}
		}
		camera.move(loop_time);
		camera.setMatrixSenderViewMatrix();
		MatrixSender::CalculateMVP();
		MatrixSender::SendMVP();
		SDL_GL_SwapWindow(display);
		loop_time = SDL_GetTicks() - time;
		time += loop_time;
	} while(cont);
}

int main() {
	SDL_Window *display;
	//SDL_GLContext context;
	SDL_CATCH(SDL_Init(SDL_INIT_EVERYTHING));
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	display = SDL_CreateWindow("20 Pin Bowling",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			kWindowWidth,kWindowHeight,SDL_WINDOW_OPENGL);
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE));

	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4));
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0));
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));

	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
	//static SDL_GLContext contex =
	SDL_GL_CreateContext(display);
	SDL_CATCH(SDL_GL_SetSwapInterval(1));
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "GLEW Error!!");
		return -1;
	}
	//HACK: GLEW Pushes a GL_INVALID_ENUM error on init, glGetError dequeues it 
	glGetError();
	glClearColor(0.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	atexit(SDL_Quit);
	main_loop(display);
	return 0;
}
