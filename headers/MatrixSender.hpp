#pragma once

#include <GL/glew.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <SDL2/SDL_assert.h>
class MatrixSender {
	static glm::mat4 MVP , Projection, Model, View;
	static GLuint ShaderMVPLocation;
	public:
	static inline void Init(GLuint mvp_location) {
		SDL_assert(~mvp_location);
		ShaderMVPLocation = mvp_location;
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
	static inline void SetMVP() {
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
};
