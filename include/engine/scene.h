#pragma once
#include <GLFW/glfw3.h>

// #include "camera.h"
// #include "world.h"
class Camera;
// class World;
class Entity;

class Scene {
    GLFWwindow *window;
    Camera *camera;
    World *world;
public:
    std::vector<std::vector<int>> map;
    std::vector<Entity*> entities;
    std::vector<Entity*> selectedEntities;
    std::vector<int> transperents;
    Scene(GLFWwindow *window, Camera *camera);
    void addEntity(Entity* entity);
    void selectEntity(Entity* entity);

    void clearSelectedEntity();
    std::vector<Entity*> getSelectedEntity();
    std::vector<Entity*> getEntities() const;

    void setEntities(std::vector<Entity*> entities);
    void removeEntity(Entity* entity);

    std::vector<Entity*> getEntitiesWithinDistance(float distance);

    Entity* getEntityByPosition(const glm::vec3 &pos);

    void update();

    void initWorld(World *world);
    void updateWorld();
    bool isVisible(Entity* target, std::vector<Entity*>& entities);
};