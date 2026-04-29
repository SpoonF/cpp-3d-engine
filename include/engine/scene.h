#pragma once
#include <GLFW/glfw3.h>

class Camera;
class World;
class Player;


class Scene {
private:
    GLFWwindow *window;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<World> world;
    std::shared_ptr<Player> player;
    float lastTime;
    Frustum frustum;
public:
    std::vector<int> transperents;
    Scene(GLFWwindow *window, std::shared_ptr<Camera> camera, std::shared_ptr<Player> player);

    void update();

    void setWorld(std::shared_ptr<World> world);
    void updateWorld();
};