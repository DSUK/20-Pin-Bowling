#include "debug.hpp"
void gl_error(int number, int line, const char *file) {
	const char *error;
	switch(number) {
		case GL_INVALID_ENUM:
			error = static_cast<const char *>("GL Invalid Enum");
		break;
		case GL_INVALID_VALUE:
			error = static_cast<const char *>("GL Invalid Value");
		break;
		case GL_INVALID_OPERATION:
			error = static_cast<const char *>("GL Invalid Operation");
		break;
		case GL_STACK_OVERFLOW:
			error = static_cast<const char *>("GL Stack Overflow");
		break;
		case GL_STACK_UNDERFLOW:
			error = static_cast<const char *>("GL Stack Underflow");
		break;
		case GL_OUT_OF_MEMORY:
			error = static_cast<const char *>("GL Out Of Memory");
		break;
		case GL_TABLE_TOO_LARGE:
			error = static_cast<const char *>("GL Table Too Large");
		break;
	}
	SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "OpenGL Error: %s on line %d of %s : %s\n",error,line,file,SDL_GetError());
	exit(number);
}
