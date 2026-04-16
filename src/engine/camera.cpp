#include "engine/camera.h"
#include <cstdio>

Camera::Camera(GLFWwindow* window) {
    position = {0,0,0};
    this->window = window;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwGetWindowSize(window, &width, &height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    this->lastTime = glfwGetTime();
}

glm::mat4 Camera::updateView(float deltaTime) {
    glfwGetCursorPos(window, &xpos, &ypos);

    horizontalAngle += settings.mouseSpeed * deltaTime * float(width/2 - xpos);
    verticalAngle += settings.mouseSpeed * deltaTime * float(height/2 - ypos);

    verticalAngle = std::min(verticalAngle, 1.5f);
    verticalAngle = std::max(verticalAngle, -1.5f);

    glm::vec3 direction(
        cos(verticalAngle) * sin(horizontalAngle),
        sin(verticalAngle),
        cos(verticalAngle) * cos(horizontalAngle)
    );

    glm::vec3 right(
        sin(horizontalAngle - 3.14f/2.0f),
        0,
        cos(horizontalAngle - 3.14f/2.0f)
    );

    glm::vec3 up = glm::cross( right, direction );

    for (size_t i = 0; i < callbacks_.size(); i++) {
        callbacks_[i](direction, right, up, deltaTime);
    }

    glfwSetCursorPos(window, width/2, height/2);

    return glm::lookAt(
        position,
        position + direction,
        up
    );
}

glm::mat4 Camera::updateProjection() {
    return glm::perspective(glm::radians(settings.fov), 4.f/3.f, 0.1f, 300.0f);
}

void Camera::setViewCallback(CallbackType action) {
    callbacks_.push_back(action);
}

void Camera::setPosition(const glm::vec3& pos)  {
    this->position = pos;
}

glm::vec3 Camera::getPosition()  {
    return this->position;
}

bool Camera::isWithinDistance(const Object &other, float distance) const
{
    glm::vec3 dist = this->position - other.getPosition();

    bool viewInDinstance = abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance;

    if(viewInDinstance) {
        return false;
    }
    return true;
}

bool Camera::isWithinDistance(const Chunk &other, float distance) const
{
    glm::vec3 dist = this->position - other.position;

    bool viewInDinstance = abs(dist.x) > distance * 2 * CHUNK_WIDTH || abs(dist.z) > distance * 2 * CHUNK_WIDTH;

    if(viewInDinstance) {
        return false;
    }
    return true;
}

void Camera::update()
{
    double currentTime = glfwGetTime();
    this->deltaTime = float(currentTime - this->lastTime);
    this->lastTime = currentTime;

    this->view = this->updateView(this->deltaTime);
    this->projection = this->updateProjection();

}

glm::mat4 Camera::getView() {
    return this->view;
}
glm::mat4 Camera::getProjection() {
    return this->projection;
}