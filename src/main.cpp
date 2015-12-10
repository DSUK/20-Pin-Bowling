#define GLM_FORCE_RADIANS
#ifndef GL3_PROTOTYPES
#define GL3_PROTOTYPES 1
#endif
#include <GL/glew.h>
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
#include "debug.hpp"

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

}
void main_loop(SDL_Window *display) {
	//GLCamera cam(glm::vec3(0,0,2));
	GLSLLoader shaders;
	shaders.loadFile(GL_VERTEX_SHADER,"./glsl/test.vert");
	shaders.loadFile(GL_FRAGMENT_SHADER,"./glsl/test.frag");
	shaders.compile(GL_VERTEX_SHADER);
	shaders.compile(GL_FRAGMENT_SHADER);
	shaders.attach(GL_VERTEX_SHADER);
	shaders.attach(GL_FRAGMENT_SHADER);
	shaders.bindAttribute(0, (char*)"in_vertexpos");
	shaders.link();
	shaders.useProgram();
	SDL_Event event;
	bool cont = true;
	GLuint VertexArrayId;
	glGenVertexArrays(1,&VertexArrayId);
	glBindVertexArray(VertexArrayId);

	GLuint vertexBuffer;
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		1.0f, -1.0f, 0.0f,
		0.0f, 1.0f, 0.0f
	};
	glEnableVertexAttribArray(0);
	glGenBuffers(1,&vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
	do {
		shaders.useProgram();
		GL_CATCH();
		glClear(GL_COLOR_BUFFER_BIT);
		GL_CATCH();
		GL_CATCH();
		glBindBuffer(GL_ARRAY_BUFFER,vertexBuffer);
		GL_CATCH();
		glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
		GL_CATCH();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		GL_CATCH();
		//glDisableVertexAttribArray(0);

		while(SDL_PollEvent(&event))
		{
			if(event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				cont = false;
		};
		SDL_GL_SwapWindow(display);

	} while(cont);
	SDL_Quit();
}

//void mainloop(SDL_Window *display) {
void unreachable(SDL_Window *display) {

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

/*int main() {
	SDL_Window *display;
	//static SDL_GLContext context;
	int value;
	SDL_CATCH(SDL_Init(SDL_INIT_EVERYTHING));
	SDL_LogSetPriority(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO);
	display = SDL_CreateWindow("20 Pin Bowling",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,kWindowWidth,kWindowHeight,SDL_WINDOW_OPENGL|SDL_WINDOW_SHOWN);
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,4));
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,0));
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1));
	SDL_CATCH(SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24));
	SDL_CATCH(SDL_GL_GetAttribute(SDL_GL_DOUBLEBUFFER, &value));
	if(SDL_GL_CreateContext(display) == NULL)
		exit(69);
	glClearColor(0.0,1.0,1.0,1.0);
	glEnable(GL_DEPTH_TEST);
	main_loop(display);
	return 0;
}*/




class Shader
{
public:
	std::string ReadFile(const char* file)
	{
		// Open file
		std::ifstream t(file);

		// Read file into buffer
		std::stringstream buffer;
		buffer << t.rdbuf();

		// Make a std::string and fill it with the contents of buffer
		std::string fileContent = buffer.str();

		return fileContent;
	}

	void BindAttributeLocation(int index, const std::string &attribute)
	{
		// Bind attribute index 0 (coordinates) to in_Position and attribute index 1 (color) to in_Color
		// Attribute locations must be setup before calling glLinkProgram
		glBindAttribLocation(shaderProgram, index, attribute.c_str());
	}

	void UseProgram()
	{
		// Load the shader into the rendering pipeline
		glUseProgram(shaderProgram);
	}

	bool Init()
	{
		// Generate our shader. This is similar to glGenBuffers() and glGenVertexArray(), except that this returns the ID
		shaderProgram = glCreateProgram();

		if (!LoadVertexShader("tutorial2.vert"))
			return false;

		if (!LoadFragmentShader("tutorial2.frag"))
			return false;

		// All shaders has been create, now we must put them together into one large object
		return LinkShaders();
	}


	bool LoadVertexShader(const std::string &filename)
	{
		std::cout << "Linking Vertex shader" << std::endl;

		// Read file as std::string
		std::string str = ReadFile(filename.c_str());

		 // c_str() gives us a const char*, but we need a non-const one
		const char* src = str.c_str();
		int32_t size = str.length();

		// Create an empty vertex shader handle
		vertexshader = glCreateShader(GL_VERTEX_SHADER);

		// Send the vertex shader source code to OpenGL
		glShaderSource(vertexshader, 1, &src, &size);

		// Compile the vertex shader
		glCompileShader(vertexshader);

		int wasCompiled = 0;
		glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &wasCompiled );

		if (wasCompiled == 0)
		{
			PrintShaderCompilationErrorInfo(vertexshader);
			return false;
		}

		glAttachShader(shaderProgram, vertexshader);
		return true;
	}

	bool LoadFragmentShader(const std::string &filename)
	{
		std::cout << "Loading Fragment Shader" << std::endl;

		// Read file as std::string
		std::string str = ReadFile(filename.c_str());

		 // c_str() gives us a const char*, but we need a non-const one
		const char* src = str.c_str();
		int32_t size = str.length();

		// Create an empty vertex shader handle
		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

		// Send the vertex shader source code to OpenGL
		glShaderSource(fragmentShader, 1, &src, &size);

		// Compile the vertex shader
		glCompileShader(fragmentShader);

		int wasCompiled = 0;
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &wasCompiled );

		if (wasCompiled == false)
		{
			PrintShaderCompilationErrorInfo(fragmentShader);
			return false;
		}

		glAttachShader(shaderProgram, fragmentShader);
		return true;
	}

	bool LinkShaders()
	{
		// Link. At this point, our shaders will be inspected/optized and the binary code generated
		// The binary code will then be uploaded to the GPU
		glLinkProgram(shaderProgram);

		// Verify that the linking succeeded
		int isLinked;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, (int *)&isLinked);

		if (isLinked == false)
			PrintShaderLinkingError(shaderProgram);

		return isLinked != 0;
	}

	void PrintShaderLinkingError(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader linking failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetProgramiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		std::cout << "Info Length : " << maxLength << std::endl;

		// Get shader info log
		char* shaderProgramInfoLog = new char[maxLength];
		glGetProgramInfoLog(shaderProgram, maxLength, &maxLength, shaderProgramInfoLog);

		std::cout << "Linker error message : " << shaderProgramInfoLog << std::endl;

		/* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
		/* In this simple program, we'll just leave */
		delete shaderProgramInfoLog;
		return;
	}

	// If something went wrong whil compiling the shaders, we'll use this function to find the error
	void PrintShaderCompilationErrorInfo(int32_t shaderId)
	{
		std::cout << "=======================================\n";
		std::cout << "Shader compilation failed : " << std::endl;

		// Find length of shader info log
		int maxLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &maxLength);

		// Get shader info log
		char* shaderInfoLog = new char[maxLength];
		glGetShaderInfoLog(shaderId, maxLength, &maxLength, shaderInfoLog );

		// Print shader info log
		std::cout << "\tError info : " << shaderInfoLog << std::endl;

		std::cout << "=======================================\n\n";
		delete shaderInfoLog;
	}

	void CleanUp()
	{
		/* Cleanup all the things we bound and allocated */
		glUseProgram(0);
		glDetachShader(shaderProgram, vertexshader);
		glDetachShader(shaderProgram, fragmentShader);

		glDeleteProgram(shaderProgram);


		glDeleteShader(vertexshader);
		glDeleteShader(fragmentShader);
	}

	// The handle to our shader program
	GLuint shaderProgram;

	// The handles to the induvidual shader
	GLuint vertexshader, fragmentShader;

};
std::string programName = "Headerphile SDL2 - OpenGL thing";


// Our opengl context handle
SDL_GLContext mainContext;

// Our object has 4 points
const uint32_t points = 4;

// Each poin has three values ( x, y, z)
const uint32_t floatsPerPoint = 3;

// Each color has 4 values ( red, green, blue, alpha )
const uint32_t floatsPerColor = 4;

// This is the object we'll draw ( a simple square
const GLfloat diamond[points][floatsPerPoint] = {
	{ -0.5,  0.5,  0.5 }, // Top left
	{  0.5,  0.5,  0.5 }, // Top right
	{  0.5, -0.5,  0.5 }, // Bottom right 
	{ -0.5, -0.5,  0.5 }, // Bottom left
};

// This is the object we'll draw ( a simple square
const GLfloat colors[points][floatsPerColor] = {
	{ 0.0, 1.0, 0.0, 1.0 }, // Top left
	{ 1.0, 1.0, 0.0, 1.0  }, // Top right
	{ 1.0, 0.0, 0.0, 1.0  }, // Bottom right 
	{ 0.0, 0.0, 1.0, 1.0  }, // Bottom left
};

// Create variables for storing the ID of our VAO and VBO
GLuint vbo[2], vao[1];

// The positons of the position and color data within the VAO
const uint32_t positionAttributeIndex = 0, colorAttributeIndex = 1;

// Our wrapper to simplify the shader code
Shader shader;

bool SetOpenGLAttributes();
void PrintSDL_GL_Attributes();
void CheckSDLError(int line);

void Render(SDL_Window *mainWindow)
{
	// First, render a square without any colors ( all vertexes will be black )
	// ===================
	// Make our background grey
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
	glDrawArrays(GL_LINE_LOOP, 0, 4);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(mainWindow);

	std::cout << "Press ENTER to render next frame\n";
	std::cin.ignore();

	// Second, enable the colors and draw a solid square
	// ===================
	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(colorAttributeIndex);

	// Make our background black
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Invoke glDrawArrays telling that our data is a line loop and we want to draw 2-4 vertexes
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	// Swap our buffers to make our changes visible
	SDL_GL_SwapWindow(mainWindow);

}
bool SetupBufferObjects()
{
	// Generate and assign two Vertex Buffer Objects to our handle
	glGenBuffers(2, vbo);

	// Generate and assign a Vertex Array Object to our handle
	glGenVertexArrays(1, vao);

	// Bind our Vertex Array Object as the current used object
	glBindVertexArray(vao[0]);

	// Positions
	// ===================
	// Bind our first VBO as being the active buffer and storing vertex attributes (coordinates)
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerPoint) * sizeof(GLfloat), diamond, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(positionAttributeIndex, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable our attribute within the current VAO
	glEnableVertexAttribArray(positionAttributeIndex);

	// Colors
	// =======================
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	// Copy the vertex data from diamond to our buffer
	glBufferData(GL_ARRAY_BUFFER, ( points * floatsPerColor) * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	// Specify that our coordinate data is going into attribute index 0, and contains three floats per vertex
	glVertexAttribPointer(colorAttributeIndex, 4, GL_FLOAT, GL_FALSE, 0, 0);

	// Note : We didn't enable the colors here!

	// Set up shader ( will be covered in the next part )
	// ===================
	if (!shader.Init())
		return false;

	shader.UseProgram();

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	return true;
}
bool Init(SDL_Window **mainWindow)
{
	// Initialize SDL's Video subsystem
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		std::cout << "Failed to init SDL\n";
		return false;
	}

	// Create our window centered at 512x512 resolution
	*mainWindow = SDL_CreateWindow(programName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			512, 512, SDL_WINDOW_OPENGL);

	// Check that everything worked out okay
	if (!mainWindow )
	{
		std::cout << "Unable to create window\n"<< std::endl;;
		CheckSDLError(__LINE__);
		return false;
	}

	SetOpenGLAttributes();

	// Create our opengl context and attach it to our window
	mainContext = SDL_GL_CreateContext(*mainWindow );

	// This makes our buffer swap syncronized with the monitor's vertical refresh
	SDL_GL_SetSwapInterval(1);

	// Init GLEW
	glewExperimental = GL_TRUE;
	glewInit();

	return true;
}

bool SetOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

	// Turn on double buffering with a 24bit Z buffer.
	// You may need to change this to 16 or 32 for your system
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	return true;
}

void Cleanup(SDL_Window **mainWindow)
{
	// Cleanup all the things we bound and allocated
	shader.CleanUp();

	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);

	// Delete our OpengL context
	SDL_GL_DeleteContext(mainContext);

	// Destroy our window
	SDL_DestroyWindow(*mainWindow );

	// Shutdown SDL 2
	SDL_Quit();
}

int main(int argc, char *argv[])
{
	SDL_Window *mainWindow;
	if (!Init(&mainWindow))
		return -1;

	// Clear our buffer with a grey background
	glClearColor(0.5, 0.5, 0.5, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	SDL_GL_SwapWindow(mainWindow);

	std::cout << "Seting up VBO + VAO..." << std::endl;
	if (!SetupBufferObjects())
		return -1;

	std::cout << "Rendering..." << std::endl;
	Render(mainWindow);

	std::cout << "Rendering done!\n";
	std::cin.ignore();

	Cleanup(&mainWindow);

	return 0;
}

void CheckSDLError(int line = -1)
{
	std::string error = SDL_GetError();

	if (error != "")
	{
		std::cout << "SLD Error : " << error << std::endl;

		if (line != -1)
			std::cout << "\nLine : " << line << std::endl;

		SDL_ClearError();
	}
}

void PrintSDL_GL_Attributes()
{
	int value = 0;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MAJOR_VERSION : " << value << std::endl;

	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, &value);
	std::cout << "SDL_GL_CONTEXT_MINOR_VERSION: " << value << std::endl;
}
