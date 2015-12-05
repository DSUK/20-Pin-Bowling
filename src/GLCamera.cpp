#include "GLCamera.hpp"
GLCamera::GLCamera(glm::vec3 pos) {
    position = pos;
    yRotation = 0.0;
    xzRotation = 0.0;
    upVector = glm::vec3(0.0,1.0,0.0);
    lookVector = glm::vec3(0.0,0.0,-1.0);
    leftVector = glm::cross(upVector,lookVector);
}

glm::mat4 GLCamera::calculateViewMatrix() {
    return glm::lookAt(position,position + lookVector, upVector);
}

void GLCamera::rotateXY(int x, int y) {
        xzRotation += static_cast<float>(x)/360;
        yRotation -= static_cast<float>(y)/360;
        float sin_xz = sin(xzRotation);
        float cos_xz = cos(xzRotation);
        //xz_deg = xzRotation*180/M_PI;
        float y_deg = yRotation*180/M_PI;
        leftVector = glm::vec3(cos_xz,0,sin_xz);
        glm::vec3 temp_vec = glm::vec3(sin_xz,0,-cos_xz); //"face vector"
        upVector = glm::rotate(temp_vec,y_deg+90,leftVector);
        lookVector = glm::rotate(temp_vec,y_deg,leftVector);
}

void GLCamera::setFowardMove(float amount) {
    upMove = amount;
}

void GLCamera::setLeftMove(float amount) {
    leftMove = -amount;
}

void GLCamera::move() {
    position += lookVector*upMove + leftMove*leftVector;
}

glm::vec3 GLCamera::getLook() {
    return lookVector;
}

glm::vec3 GLCamera::getPos() {
    return position;
}
