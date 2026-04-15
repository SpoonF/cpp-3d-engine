#include "engine/scene.h"

#include "engine/shader.h"
#include "config.h"

#include <chrono>
#include <iostream>

class Profiler {
private:
    std::chrono::steady_clock::time_point start;
    std::string name;
    
public:
    Profiler(const std::string& n) : name(n) {
        start = std::chrono::steady_clock::now();
    }
    
    ~Profiler() {
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
        printf("%s: %.2f FPS\n", name.c_str(), 1000.0 / (duration * 0.001));
    }
};

Scene::Scene(GLFWwindow *window, std::shared_ptr<Camera> camera) {
    this->window = window;
    this->camera = camera;
    this->lastTime = glfwGetTime();
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

        // Object3D object = entity->getObject();
        // glm::vec3 position = entity->getPosition();

        // if(entity->isType(EntityType::LIGHT)) {
        //     lightPos = position;
        // }

        // if(map.count(object) == 0) {
        //     ShaderOptions options{ {position} };
        //     map[object] = options;
        // }else {
        //     map[object].positions.push_back(position);
        // }
        
    }

    // std::cout << map.size() << std::endl;

    for (const auto &[object, options] : map)
    {
        object.shader->set("lightPos", lightPos);
        object.render(options);
    }
}
void Scene::setWorld(std::shared_ptr<World> world)
{
    this->world = world;
}

void Scene::updateWorld()
{
    Profiler p("Full render");
    std::map<std::shared_ptr<Object3D>, ShaderOptions> map;
    glm::vec3 lightPos;

    std::vector<std::shared_ptr<Chunk>> chunks = this->world->getChunks(); 

    for (auto &chunk : chunks)
    {
        if(!camera->isWithinDistance(*chunk, RENDER_DISTANCE)) {
            continue;
        }

        for (auto const &[object, positions] : chunk->objects)
        {
            object->shader->set("lightPos", lightPos);
            ShaderOptions options;
            options.positions = positions;
            object->render(options);
        }
    }
    

    // for (auto const &chunk : chunks)
    // {
    //     if(!camera->isWithinDistance(*chunk, RENDER_DISTANCE)) {
    //         continue;
    //     }

    //     auto quads = chunk->greedy_mesh_face(1);

        
    //     // for (auto const &quad : quads)
    //     // {

    //     //     // if(entity->isType(EntityType::LIGHT)) {
    //     //     //     lightPos = position;
    //     //     // }

    //     //     if(map.count(quad.object) == 0) {
                
    //     //         ShaderOptions options{ {quad.pos} };
    //     //         map[quad.object] = options;
    //     //     }else {
    //     //         map[quad.object].positions.push_back(quad.pos);
    //     //     }
    //     // }
        

    //     // for (auto const &entity : chunk->getEntities())
    //     // {
    //     //     if(entity == nullptr) {
    //     //         continue;
    //     //     }

    //     //     std::shared_ptr<Object3D> object = entity->getObject();
    //     //     glm::vec3 position = entity->getPosition();

    //     //     // if(entity->isType(EntityType::LIGHT)) {
    //     //     //     lightPos = position;
    //     //     // }

    //     //     if(map.count(object) == 0) {
    //     //         ShaderOptions options{ {position} };
    //     //         map[object] = options;
    //     //     }else {
    //     //         map[object].positions.push_back(position);
    //     //     }
            
    //     // }
    // }

    // for (const auto &[object, options] : map)
    // {
    //     object->shader->set("lightPos", lightPos);
    //     object->render(options);
    // }
}

bool Scene::isVisible(Entity* target, std::vector<Entity*>& entities) {

    return true;
}
