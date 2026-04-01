#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <functional>
#include <map>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// #include "../include/engine.h"
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

// class EntityCompentities {
// private:
//     Object3D object;
// public:
//     std::vector<glm::vec3> positions;
//     EntityCompentities(const Object3D& object) {
//         this->object = object;
//     }
//     EntityCompentities(const Object3D& object, glm::vec3 position) {
//         this->object = object;
//         this->addPosition(position);
//     }
//     void addPosition(glm::vec3& position) {
//         this->positions.push_back(position);
//     };
//     std::vector<glm::vec3> getPositions() {
//         return this->positions;
//     }
//     Object3D getObject() {
//         return this->object;
//     };
// };

class Scene {
    GLFWwindow* window;
    public:
    std::vector<std::vector<int>> map;
    std::vector<Entity*> entities;
    std::vector<Entity*> selectedEntities;
    std::vector<int> transperents;
    Scene(GLFWwindow *window) {
        this->window = window;
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

    void update() {

        std::map<Object3D, ShaderOptions> map;

        for (auto const &entity : entities)
        {

            Object3D object = entity->getObject();
            glm::vec3 position = entity->getPosition();

            if(entity->getType() == EntityType::LIGHT) {
                // shdr->set("lightPos", position);
            }

            size_t j = 0;

            if(map.count(object) == 0) {
                ShaderOptions options{ {position} };
                // map.insert({object, options});
                map[object] = options;
            }else {
                map[object].positions.push_back(position);
            }
           
        }

        std::cout << map.size() << std::endl;

        for (const auto &[object, options] : map)
        {
            object.render(options);
        }

        
    }
};
