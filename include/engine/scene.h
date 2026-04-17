#pragma once
#include <GLFW/glfw3.h>

class Camera;
class World;


class Scene {
private:
    GLFWwindow *window;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<World> world;
    float lastTime;
    Frustum frustum;
public:
    std::vector<int> transperents;
    Scene(GLFWwindow *window, std::shared_ptr<Camera> camera);

    void update();

    void setWorld(std::shared_ptr<World> world);
    void updateWorld();
};