#pragma once

#define _ISOC11_SOURCE 1
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <GL/glew.h>
class GLSLLoader {
	std::string fragSource;
	std::string vertexSource;
	GLuint vertexShader;
	GLuint fragmentShader;
	GLuint programObject;

	public:
	GLSLLoader();
	void loadFile(GLenum shader_type, const GLchar* source_file);
	void bindAttribute(GLint location, const GLchar* name);
	void compile(GLenum shader_type);
	void link();
	void attach(GLenum shader_type);
	void remove(GLenum shader_type);
	void useProgram();
	GLuint uniformLocation(const GLchar *name);
	GLuint getProgramObject();
	~GLSLLoader();
};
