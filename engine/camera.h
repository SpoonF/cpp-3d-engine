#ifndef CAMERA_H
#define CAMERA_H

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/quaternion.hpp>

#include <algorithm>
#include <cmath>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera  {
    glm::vec3 position;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float fov = 70.0f;
    float mouseSpeed = 0.05f;
    double xpos, ypos;
    int width, height;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    GLFWwindow* window;
    public: 
    using CallbackType = std::function<void( glm::vec3, glm::vec3, glm::vec3, float)>;
    Camera(GLFWwindow* window);
    glm::mat4 getView(float deltaTime);
    glm::mat4 getProjection();
    void setViewCallback(CallbackType action);
    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition();
private:
    std::vector<CallbackType> callbacks_;
};

#endif


