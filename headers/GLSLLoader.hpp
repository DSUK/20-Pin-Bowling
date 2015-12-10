#pragma once

#define GL_GLEXT_PROTOTYPES 1
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <SDL2/SDL_opengl.h>
class GLSLLoader {
	std::string fragSource;
	std::string vertexSource;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;

	public:
	GLSLLoader();
	void loadFile(GLenum shader_type, const GLchar* source_file);
	void compile(GLenum shader_type);
	void link();
	void attach(GLenum shader_type);
	void remove(GLenum shader_type);
	void useProgram();
	GLuint getProgramObject();
	~GLSLLoader();
};
