#include "GLSLLoader.hpp"
//TODO: add file watchers
GLSLLoader::GLSLLoader() {
	programObject = glCreateProgram();
}

void GLSLLoader::loadFile(GLenum shader_type, const GLchar* source_file) {
	std::ifstream source(source_file);
	if(!source) {
		std::cerr << "problem opening the file : " << source_file << std::endl;
		return;
	}

	std::ostringstream stream;
	stream << source.rdbuf();
	switch(shader_type) {

		case GL_FRAGMENT_SHADER:
			fragSource = stream.str();
		break;
		case GL_VERTEX_SHADER:
			vertexSource = stream.str();
		default:
		break;
	}
}

void GLSLLoader::compile(GLenum shader_type) {
	GLint status = 0;
	switch(shader_type) {
		case GL_FRAGMENT_SHADER: {
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const GLchar* data = fragSource.c_str();
			glShaderSource(fragmentShader, 1, &data, NULL);
			glCompileShader(fragmentShader);

			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE) {
				GLint max_log_length = 0;
				glGetShaderiv(fragmentShader,GL_INFO_LOG_LENGTH, &max_log_length);

				std::vector<char> errorlog(max_log_length);
				glGetShaderInfoLog(fragmentShader, max_log_length, &max_log_length, &errorlog[0]);

				std::cerr << "Fragment error : " << &errorlog[0] <<std::endl;
				glDeleteShader(fragmentShader);
			}
		}
		break;
		case GL_VERTEX_SHADER: {

			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const GLchar* data = vertexSource.c_str();
			glShaderSource(vertexShader, 1, &data, NULL);
			glCompileShader(vertexShader);

			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE) {

				GLint max_log_length = 0;
				glGetShaderiv(vertexShader,GL_INFO_LOG_LENGTH, &max_log_length);

				std::vector<char> errorlog(max_log_length);
				glGetShaderInfoLog(vertexShader,max_log_length,&max_log_length,&errorlog[0]);

				std::cerr << "Vertex error : " << &errorlog[0] <<std::endl;
				glDeleteShader(vertexShader);
			}
		}
		default:
		break;
	}
}

void GLSLLoader::link() {

	GLint okay;
	glLinkProgram(programObject);
	glGetProgramiv(programObject,GL_LINK_STATUS,&okay);

	if(!okay) {
		GLint max_log_length = 0;
		glGetProgramiv(programObject, GL_INFO_LOG_LENGTH, &max_log_length);

		std::vector<char> errorlog(max_log_length);
		glGetProgramInfoLog(programObject, max_log_length, &max_log_length, &errorlog[0]);

		std::cerr << "Linker error : " << &errorlog[0] <<std::endl;
		glDeleteProgram(programObject);
		exit(okay);
	}
}

void GLSLLoader::attach(GLenum shader_type) {
	switch(shader_type) {
		case GL_FRAGMENT_SHADER:
			glAttachShader(programObject,fragmentShader);
		break;
		case GL_VERTEX_SHADER:
			glAttachShader(programObject,vertexShader);
		default:
		break;
	}
}

void GLSLLoader::remove(GLenum shader_type) {
	switch(shader_type) {
		case GL_FRAGMENT_SHADER:
			glDeleteShader(fragmentShader);
		break;
		case GL_VERTEX_SHADER:
			glDeleteShader(vertexShader);
		break;
		default:
		break;
	}
}
void GLSLLoader::useProgram() {
	glUseProgram(programObject);
}
GLuint GLSLLoader::getProgramObject() {
	return programObject;
}

GLSLLoader::~GLSLLoader() {
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteProgram(programObject);
}
