#pragma once

extern void gl_error(int number, int line, const char *file);
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <execinfo.h>
#define STACK_TRACE_LENGTH 10
#define SDL_CATCH(X) \
	do {	int _____TEMP; \
		if((_____TEMP = X)) {\
			SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed " #X "on line %d of %s : %s\n", __LINE__,__FILE__,SDL_GetError()); \
			exit(_____TEMP); \
	} }while(0)

#define GL_CATCH() \
	do {	int _____TEMP; \
		if(((_____TEMP = glGetError()) != GL_NO_ERROR)) {\
			gl_error(_____TEMP,__LINE__,__FILE__); \
		}\
	} while(0)
