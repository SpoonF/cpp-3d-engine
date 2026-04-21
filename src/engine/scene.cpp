#include "engine/scene.h"

#include "engine/object.h"
#include "engine/object/block.h"
#include "engine/shader.h"
#include "engine/camera.h"
#include "engine/world.h"
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
        printf("%s: %.4f ms\n", name.c_str(), duration/ 1000.0);
    }
};

Scene::Scene(GLFWwindow *window, std::shared_ptr<Camera> camera) {
    this->window = window;
    this->camera = camera;
    this->lastTime = glfwGetTime();
    printf("Create scene");
}

void Scene::update() {

    std::map<Object, ShaderOptions> map;
    glm::vec3 lightPos;
    

    // for (auto const &entity : entities)
    // {
    //     // if(!camera->isWithinDistance(*entity, RENDER_DISTANCE * 2)) {
    //     //     continue;
    //     // }

    //     // Object3D object = entity->getObject();
    //     // glm::vec3 position = entity->getPosition();

    //     // if(entity->isType(EntityType::LIGHT)) {
    //     //     lightPos = position;
    //     // }

    //     // if(map.count(object) == 0) {
    //     //     ShaderOptions options{ {position} };
    //     //     map[object] = options;
    //     // }else {
    //     //     map[object].positions.push_back(position);
    //     // }
        
    // }

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

    std::unordered_map<ObjectType, ShaderOptions> map;
    glm::vec3 lightPos;

    std::vector<std::shared_ptr<Chunk>> chunks = this->world->getChunks();
    std::vector<Object*> lights = this->world->getLights();

    for (auto &chunk : chunks)
    {

        if(!camera->isWithinDistance(*chunk, RENDER_DISTANCE)) {
            continue;
        }

        glm::vec3 min = chunk->position;

        glm::vec3 max(min.x + CHUNK_WIDTH * 2, CHUNK_HEIGHT, min.z + CHUNK_WIDTH * 2);

        if(!this->frustum.isAABBVisible(min, max)) {
            continue;
        }

        for (auto &[key, positions] : chunk->getPositions())
        {

            if(map.count(key) == 0) {
                ShaderOptions options{ positions };
                map[key] = options;
            }else {
                map[key].positions.insert(map[key].positions.end(), positions.begin(), positions.end());
            }
        }
    }

    // std::cout << map.size() << std::endl;


    for (const auto &[type, options] : map)
    {


        if (type == ObjectType::BLOCK)
        {
            auto shader = Shader::getInstance(ObjectType::BLOCK);
            auto lightPos = lights[0]->getPosition();
            shader->set("lightPos", lightPos);
            shader->drawObjectInstaced(Model::getInstance(ObjectType::BLOCK), imageBMP::getInstance(ObjectType::BLOCK), options);
        }
        

        
    }
}

