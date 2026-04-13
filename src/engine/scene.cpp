#include "engine/scene.h"

#include "engine/shader.h"
#include "config.h"

Scene::Scene(GLFWwindow *window, Camera *camera) {
    this->window = window;
    this->camera = camera;
}
void Scene::addEntity(Entity* entity) {
    entities.push_back(entity);
    
}
void Scene::selectEntity(Entity* entity) {
    entity->isSelected = true;
}

void Scene::clearSelectedEntity() {
    for (size_t i = 0; i < entities.size(); i++)
    {
        entities[i]->isSelected = false;
    }
    
}
std::vector<Entity*> Scene::getSelectedEntity() {
    selectedEntities.clear();
    for (size_t i = 0; i < entities.size(); i++)
    {
        if(entities[i]->isSelected == true) {
            selectedEntities.push_back(entities[i]);
        }
    }
    return selectedEntities;
}
std::vector<Entity*> Scene::getEntities() const {
    return entities;
}

void Scene::setEntities(std::vector<Entity*> entities) {
    this->entities = entities;
}
void Scene::removeEntity(Entity* entity) {
    for (auto it = this->entities.begin(); it < this->entities.end(); it++) {
        if((*it) == entity) {
            this->entities.erase(it);
        }
    }
}

std::vector<Entity*> Scene::getEntitiesWithinDistance(float distance) {
    std::vector<Entity*> vec;

    for (auto const &entity : entities) {
        if(camera->isWithinDistance(*entity, distance)) {
            vec.push_back(entity);
        }
    }

    return vec;
    
}

Entity* Scene::getEntityByPosition(const glm::vec3 &pos) {

    for (auto const &entity : entities) {
        if(entity->isWithinDistance(pos, 1)) {
            return entity;
        }
    }

    return nullptr;
    
}

void Scene::update() {

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
void Scene::initWorld(World *world)
{
    this->world = world;
}

void Scene::updateWorld()
{
    std::map<Object3D, ShaderOptions> map;
    glm::vec3 lightPos;

    std::vector<Chunk *> chunks = this->world->getChunks(); 

    for (auto const &chunk : chunks)
    {
        if(!camera->isWithinDistance(*chunk, RENDER_DISTANCE)) {
            continue;
        }

        for (auto const &entity : chunk->entities)
        {

            Object3D object = entity->getObject();
            glm::vec3 position = entity->getPosition();

            if(map.count(object) == 0) {
                ShaderOptions options{ {position} };
                map[object] = options;
            }else {
                map[object].positions.push_back(position);
            }
        }
    }

    for (const auto &[object, options] : map)
    {
        object.shader->set("lightPos", lightPos);
        object.render(options);
    }
}

bool Scene::isVisible(Entity* target, std::vector<Entity*>& entities) {

    glm::vec3 direction = target->getPosition() - camera->getPosition();
    float t_target = glm::length2(direction);
    direction = direction / t_target;

    printf("%i \n", entities.size());

    // for (auto &other : entities)
    // {
    //     if(other == target) {
    //         continue;
    //     }

    //     // auto oc = other->getPosition() - camPos;
    //     // auto proj = glm::dot(oc, direction);

    //     // auto t_closest = proj;

    //     // if(t_closest < 0) {
    //     //     continue;
    //     // }

    //     // float dist_sq = dot(oc, oc) - t_closest * t_closest;
    //     // if(dist_sq <= 4)  {
    //     //     auto t_hit = t_closest - std::sqrt(4 - dist_sq);

    //     //     if(t_hit >= t_target - 0.01) {
    //     //         result.push_back(target);
    //     //     }
    //     // }
    // }

    return true;
}
