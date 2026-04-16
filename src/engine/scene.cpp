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


void Scene::update() {

    std::map<Object, ShaderOptions> map;
    glm::vec3 lightPos;
    

    for (auto const &entity : entities)
    {
        // if(!camera->isWithinDistance(*entity, RENDER_DISTANCE * 2)) {
        //     continue;
        // }

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

    // for (const auto &[object, options] : map)
    // {
    //     object.shader.set("lightPos", lightPos);
    //     object.render(options);
    // }
}
void Scene::setWorld(std::shared_ptr<World> world)
{
    this->world = world;
}

void Scene::updateWorld()
{
    Profiler p("Full render");

    camera->update();
    this->frustum.update(camera->getProjection() * camera->getView());

    std::map<std::shared_ptr<Object>, ShaderOptions> map;
    glm::vec3 lightPos;

    for (auto &chunk : this->world->getChunks())
    {
        if(!camera->isWithinDistance(*chunk, RENDER_DISTANCE)) {
            continue;
        }

        glm::vec3 min = chunk->position;

        glm::vec3 max(min.x + CHUNK_WIDTH * 2, CHUNK_HEIGHT, min.z + CHUNK_WIDTH * 2);

        if(!this->frustum.isAABBVisible(min, max)) {
            continue;
        }

        for (auto &object : chunk->getObjects())
        {

            if(map.count(object) == 0) {
                ShaderOptions options{ {object->getPosition()} };
                map[object] = options;
            }else {
                map[object].positions.push_back(object->getPosition());
            }
            
        }
    }

    // std::cout << map.size() << std::endl;

    for (const auto &[object, options] : map)
    {
        object->shader->set("lightPos", lightPos);
        object->render(options);
    }
}

bool Scene::isVisible(Entity* target, std::vector<Entity*>& entities) {

    return true;
}
