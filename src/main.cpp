#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <fstream>
#include <filesystem>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/epsilon.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <memory>

#include "engine.cpp"

// g++ src/main.cpp -o game -I ./include -lGL -lglfw -lGLEW && ./game

// g++ src/main.cpp -fsanitize=address -g -o game -I ./include -lGL -lglfw -lGLEW && ./game

// clang++ src/main.cpp -o build/game -fsanitize=address -g -Iinclude -lglew32 -lglfw3 -lgdi32 -lopengl32 -luser32 ; ./build/game
// g++ src/main.cpp -o game -I ./include  -I./libs/FastNoise2/include -L./libs/FastNoise2/lib -lFastNoise -lGL -lglfw -lGLEW && ./game
// g++ src/main.cpp -o game -fsanitize=address -g -I ./include -I./libs/FastNoise2/include -L./libs/FastNoise2/lib -lFastNoise -lGL -lglfw -lGLEW && ./game

bool isProgramRuning(GLFWwindow* window) {
    return !glfwWindowShouldClose(window) &&
        glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS;
}

bool keys[1024] = {false};

// Callback для клавиатуры
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        // std::cout << "Key pressed: " << key << std::endl; // Отладка
    }
    else if (action == GLFW_RELEASE) {
        keys[key] = false;
    }
}

bool mouse_keys[8] = {false};

void mouse_key_callback(GLFWwindow* window, int button, int action, int mods) {

    if (action == GLFW_PRESS) {
        mouse_keys[button] = true;
        // std::cout << "Key pressed: " << button << std::endl; // Отладка
    }
    else if (action == GLFW_RELEASE) {
        mouse_keys[button] = false;
    }
}

bool isVec3InRange(const glm::vec3& v, const glm::vec3& min, const glm::vec3& max) {
    return (v.x >= min.x && v.x <= max.x &&
            v.y >= min.y && v.y <= max.y &&
            v.z >= min.z && v.z <= max.z);
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Ошибка инициализации GLFW\n");
        return -1;
    }

        // Настройка OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create window
    GLFWwindow* window = glfwCreateWindow(1200, 1000, "OpenGL", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Ошибка создания окна\n");
        glfwTerminate();
        return -1;
    }
    
    // Make context current
    
    glfwMakeContextCurrent(window);


    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_key_callback);

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Ошибка инициализации GLEW\n");
        return -1;
    }

    // unsigned int shaderProgram = initShaderProgram();
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glEnable(GL_CULL_FACE);
    // glDepthMask(GL_FALSE);
    glDepthFunc(GL_LESS);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);  
    glClipControl(GL_LOWER_LEFT, GL_ZERO_TO_ONE);

    glEnable(GL_STENCIL_TEST);

    std::shared_ptr<Camera> camera = std::make_shared<Camera>(window);

    Shader::init(window, camera);

    Scene *scene = new Scene(window, camera);

    Object::init<ObjectType::BLOCK>("assets/models/cube.obj", "assets/textures/ground.bmp", "shaders/block.vert", "shaders/block.frag");

    printf("Loading terrain...\n");

    std::shared_ptr<World> world = World::generate(16);
    
    printf("Terrain is load...\n");

    scene->setWorld(world);

    std::shared_ptr<Player> player = std::make_shared<Player>(world->getWorldCenter());

    player->setCamera(camera);

    std::shared_ptr<Object> lightObj = std::make_shared<Object>(glm::vec3 (46, 32, 21), ObjectType::LIGHT);
    world->addLight(lightObj.get());

    camera->setViewCallback([&lightObj](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {
        lightObj->setPosition(glm::vec3(std::sin(deltaTime) * 100000, std::cos(deltaTime) * 100, std::sin(deltaTime) * 100000));
    });


    camera->setViewCallback([&player, &scene, &world](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {
        glm::vec3 pos = player->getPosition();
        glm::vec3 oldPos = player->getPosition();

        auto op = direction.x * direction.x + direction.z * direction.z;
        glm::vec3 dir(std::clamp(direction.x / op, -1.f, 1.f), 0, std::clamp(direction.z / op, -1.f, 1.f));
        glm::vec3 rig(0,0,1);

        glm::vec3 upDir = glm::vec3(0, 2, 0);


        if (keys[GLFW_KEY_W]) {
            pos += dir * deltaTime * 32.0f;
        }
        if (keys[GLFW_KEY_S]) {
            pos -= dir * deltaTime * 16.0f;
        }

        if (keys[GLFW_KEY_D]) {
            pos += right * deltaTime * 8.0f;
        }
        if (keys[GLFW_KEY_A]) {
            pos -= right * deltaTime * 8.0f;
        }
        if (keys[GLFW_KEY_SPACE]) {
            pos += upDir * deltaTime * 4.0f;
        }
        if (keys[GLFW_KEY_LEFT_SHIFT]) {
            pos -= upDir * deltaTime  * 4.0f;
        }

        
        player->setPosition(pos);

        if(pos.y > 0 && pos.y < 256 * 2) {
            
            if(Chunk* chunk = world->getChunk(pos)) {
                // std::cout << chunk << std::endl;
                double tmpPos = 0.0;

                Block* block = new Block(glm::vec3(0.f));
                CollisionInfo info;
                bool isColliseY = false;
                bool isColliseX = false;
                bool isColliseZ = false;
                for (auto &[type, positions] : chunk->getPositions())
                {
                    if(!isColliseZ || !isColliseY || !isColliseX) {
                        for (auto &blockPos : positions)
                        {
                            block->setPosition(blockPos);
                            if(player->check(block, info)) {
                                if(info.direction.y != 0) {
                                    isColliseY = true;
                                    tmpPos = blockPos.y - info.direction.y;
                                }
                                if(info.direction.x != 0) {
                                    isColliseX = true;
                                    std::cout << info.direction.x << "|" << info.direction.y << "|" << info.direction.z << std::endl;
                                }
                                if(info.direction.x != 0) {
                                    isColliseZ = true;
                                    std::cout << info.direction.x << "|" << info.direction.y << "|" << info.direction.z << std::endl;
                                }
                            }
                        }
                    }
                    
                }
                Gravity::update(player.get(), deltaTime, tmpPos);
            } else {
                Gravity::update(player.get(), deltaTime);
            }
            
        }

        std::cout << pos.y << std::endl;
        

    });

    double lastTimeF = glfwGetTime();
    double secondF = 0;
    int frame = 0;
    // Основной цикл

    printf("[Debag]: Start updates\n");

    while (isProgramRuning(window)) {
        // Очистка экрана 135, 206, 235

        glClearColor(0.11f, 0.67f, 0.84f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glEnable(GL_CULL_FACE);

        
        scene->updateWorld();

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();

        frame += 1;
        secondF += glfwGetTime() - lastTimeF;
        lastTimeF = glfwGetTime();
        // printf("[fps]: %f f/s \n", frame/secondF);
        
    }
    
    
    // Очистка
    
    glfwTerminate();
    return 0;
};


