#pragma once

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec2.hpp> // glm::vec3
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

#include "object.h"
#include "world.h"

struct CameraSettings {
    float fov = 70.0f;
    float mouseSpeed = 0.05f;
};

class Camera  {
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
public: 
    using CallbackType = std::function<void( glm::vec3, glm::vec3, glm::vec3, float)>;
    Camera(GLFWwindow* window);

    void setViewCallback(CallbackType action);

    void setPosition(const glm::vec3& pos);
    glm::vec3 getPosition();

    bool isWithinDistance(const Object& other, float distance) const;
    bool isWithinDistance(const Chunk& other, float distance) const;

    void update();

    glm::mat4 getView();
    glm::mat4 getProjection();

protected:
    glm::mat4 view;
    glm::mat4 projection;
    double lastTime;
    double deltaTime;

private:
    glm::vec3 position;
    double xpos, ypos;
    CameraSettings settings;
    GLFWwindow* window;
    int width, height;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;

    std::vector<CallbackType> callbacks_;

    glm::mat4 updateView(float deltaTime);
    glm::mat4 updateProjection();
};




