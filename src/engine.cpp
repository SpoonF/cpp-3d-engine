#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <functional>
#include <map>
#include <algorithm>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "config.h"
#include "engine/object.cpp"
#include "engine/camera.cpp"
#include "engine/collision.cpp"
#include "engine/collision_box.cpp"
#include "engine/collision_sphere.cpp"
#include "engine/entity.cpp"
#include "engine/entity/block.cpp"
#include "engine/entity/player.cpp"
#include "engine/shader.cpp"

#include "utils/imageBMP.cpp"
#include "utils/model.cpp"

class Scene {
    GLFWwindow* window;
    Camera *camera;
public:
    std::vector<std::vector<int>> map;
    std::vector<Entity*> entities;
    std::vector<Entity*> selectedEntities;
    std::vector<int> transperents;
    Scene(GLFWwindow *window, Camera *camera) {
        this->window = window;
        this->camera = camera;
    }
    void addEntity(Entity* entity, bool isTransparent = false) {
        entities.push_back(entity);
        if(isTransparent) {
            transperents.push_back(entity->id);
        }
        
    }
    void selectEntity(Entity* entity) {
        entity->isSelected = true;
    }

    void clearSelectedEntity() {
        for (size_t i = 0; i < entities.size(); i++)
        {
            entities[i]->isSelected = false;
        }
        
    }
    std::vector<Entity*> getSelectedEntity() {
        selectedEntities.clear();
        for (size_t i = 0; i < entities.size(); i++)
        {
            if(entities[i]->isSelected == true) {
                selectedEntities.push_back(entities[i]);
            }
        }
        return selectedEntities;
    }
    std::vector<Entity*> getEntities() const {
        return entities;
    }

    void setEntities(std::vector<Entity*> entities) {
        this->entities = entities;
    }
    void removeEntity(Entity* entity) {
        for (auto it = this->entities.begin(); it < this->entities.end(); it++) {
            if((*it) == entity) {
                this->entities.erase(it);
            }
        }
    }

    std::vector<Entity*> getEntitiesWithinDistance(float distance = RENDER_DISTANCE * 2) {
        std::vector<Entity*> vec;

        for (auto const &entity : entities) {
            if(camera->isWithinDistance(*entity, distance)) {
                vec.push_back(entity);
            }
        }

        return vec;
        
    }

    Entity* getEntityByPosition(const glm::vec3 &pos) {

        for (auto const &entity : entities) {
            if(entity->isWithinDistance(pos, 1)) {
                return entity;
            }
        }

        return nullptr;
        
    }

    void update() {

        std::map<Object3D, ShaderOptions> map;
        glm::vec3 lightPos;
        

        for (auto const &entity : entities)
        {
            if(!camera->isWithinDistance(*entity, RENDER_DISTANCE * 2)) {
                continue;
            }

            Object3D object = entity->getObject();
            glm::vec3 position = entity->getPosition();

            if(entity->isType(EntityType::LIGHT)) {
                lightPos = position;
            }

            if(map.count(object) == 0) {
                ShaderOptions options{ {position} };
                map[object] = options;
            }else {
                map[object].positions.push_back(position);
            }
           
        }

        // std::cout << map.size() << std::endl;

        for (const auto &[object, options] : map)
        {
            object.shader->set("lightPos", lightPos);
            object.render(options);
        }

        
    }
};
