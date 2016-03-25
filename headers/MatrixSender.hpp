#pragma once

#include "debug.hpp"
#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <SDL2/SDL_assert.h>
//TODO: create translate, rotate and scale methods
//also setPosition (force position), setRotationalAxis (force middle, possibly bad to use), resetScale
class MatrixSender {
	static glm::mat4 MVP, Projection, Model, View, Normal;
	static glm::mat3 NormalMatrix;
	static GLuint ShaderMVPLocation;
	static GLuint NormalMVLocation;
	public:
	static inline void Init(GLuint mvp_location, GLuint shader_mv_location) {
		GL_CATCH();
		SDL_assert(~mvp_location);
		SDL_assert(~shader_mv_location);
		ShaderMVPLocation = mvp_location;
		NormalMVLocation = shader_mv_location;
	}
	static inline glm::mat4 GetMVP() {
		return MVP;
	}
	static inline glm::mat4 GetModel() {
		return Model;
	}
	static inline glm::mat4 GetView() {
		return View;
	}
	static inline glm::mat4 GetProjection() {
		return Projection;
	}
	static inline void CalculateMVP() {
		MVP = Projection*View*Model;
	}
	static inline void SetModel(glm::mat4 model) {
		Model = model;
	}
	static inline void SetView(glm::mat4 view) {
		View = view;
	}
	static inline void SetProjection(glm::mat4 projection) {
		Projection = projection;
	}
	static inline void SendMVP() {
		glUniformMatrix4fv(ShaderMVPLocation,1,GL_FALSE,&MVP[0][0]);
	}
	static inline void CalculateNormal() {
		//glm note: col first, row second
		//also: GLM documentation sucks!, just use the github
		//glm::mat4 MV = View*Model;
		//glm::mat4 MV = View*Model;
		
		//glm::mat4 MV = Model;
		glm::mat4 MV = MVP;
		
//		NormalMatrix = glm::inverseTranspose(glm::mat3(MV[0][0], MV[1][0], MV[2][0],
//							MV[0][1], MV[1][1], MV[2][1],
//							MV[0][2], MV[1][2], MV[2][2]));
		NormalMatrix = glm::inverseTranspose(glm::mat3(MV[0][0], MV[0][1], MV[0][2],
							MV[1][0], MV[1][1], MV[1][2],
							MV[2][0], MV[2][1], MV[2][2]));
	}
	static inline void SendNormal() {
		glUniformMatrix3fv(NormalMVLocation,1,GL_FALSE,&NormalMatrix[0][0]);
	}
};
