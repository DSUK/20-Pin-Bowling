#include "MatrixSender.hpp"

GLuint MatrixSender::ShaderMVPLocation = 0;
glm::mat4 MatrixSender::MVP = glm::mat4(1.0);
glm::mat4 MatrixSender::Projection = glm::mat4(1.0);
glm::mat4 MatrixSender::Model = glm::mat4(1.0);
glm::mat4 MatrixSender::View = glm::mat4(1.0);
