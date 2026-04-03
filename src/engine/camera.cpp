#include "engine/camera.h"

Camera::Camera(GLFWwindow* window) {
    position = {0,0,0};
    this->window = window;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwGetWindowSize(window, &width, &height);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

glm::mat4 Camera::getView(float deltaTime) {
    glfwGetCursorPos(window, &xpos, &ypos);

    horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos);
    verticalAngle += mouseSpeed * deltaTime * float(height/2 - ypos);

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

glm::mat4 Camera::getProjection() {
    return glm::perspective(glm::radians(fov), 4.f/3.f, 0.1f, 100.0f);
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

bool Camera::isWithinDistance(const Entity &other, float distance) const
{
    glm::vec3 dist = this->position - other.position;

    if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
        return false;
    }
    return true;
}