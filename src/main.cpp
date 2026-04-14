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

    Camera* camera = new Camera(window);

    Shader::init(window, camera);

    Scene *scene = new Scene(window, camera);

    

    

    
    printf("Loading terrain...\n");

    World *world = World::generate(8);
    
    printf("Terrain is load...\n");
    
    

    scene->initWorld(world);


    // camera->setPosition();

    // LightBlock* light = new LightBlock(glm::vec3(46, 32, 21));
    // light->setCollisable(false);

    // scene->addEntity(light);

    // float fallSpeed = 0;
    // float speedFreeFall = 0.2;
    // bool isFall = true;

    // float lastTime;
    // float moveSpeed = 0;
    // float speed = 4;
    // // bool isFly = true;
    // float jumpProgress = 0;
    // float jumpSpeed = 0.15f;
    // bool isJump = false;

    // unsigned int viewPos = 1;


    // // Где-то после создания окна

    // float speedFall = .5f;


    // auto jump = [&jumpProgress, &isJump, &isFall, &jumpSpeed, speedFreeFall](glm::vec3& pos, float deltaTime) {
    //     if(jumpProgress <= 2.0f) {
    //         if(jumpSpeed > 0.f) {
    //             jumpSpeed -= 0.002 * deltaTime;
    //         }

    //         pos.y += jumpSpeed;
    //         jumpProgress += jumpSpeed;
    //         isJump = true;
    //         isFall = false;
    //     }else {
    //         jumpProgress = 0;
    //         jumpSpeed =  0.1f;
    //         isJump = false;
    //         isFall = true;
    //     }

    // };

    // auto fall = [&isJump, &isFall, &fallSpeed, &speedFreeFall](glm::vec3& pos, float deltaTime) {
    //     if(isFall) {
    //         if(fallSpeed < 2.f) {
    //             fallSpeed += speedFreeFall * deltaTime;
    //         }

    //         pos.y -= fallSpeed;
    //     }else {
    //         fallSpeed = 0;
    //     }
    // };

    // camera->setViewCallback([&camera, speed, &scene, fall, jump, &isJump, &isFall, &viewPos, &light, &world](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {
    //     glm::vec3 pos = camera->getPosition();
    //     glm::vec3 oldPos = camera->getPosition();


    //     // printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", direction.x, direction.y, direction.z);
    //     auto op = direction.x * direction.x + direction.z * direction.z;
    //     // printf("[debag]: camera op: %f\n", op);
    //     glm::vec3 dir(std::clamp(direction.x / op, -1.f, 1.f), 0, std::clamp(direction.z / op, -1.f, 1.f));
    //     glm::vec3 rig(0,0,1);

    //     // printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", dir.x, dir.y, dir.z);

    //     if (keys[GLFW_KEY_W]) {
    //         pos += dir * deltaTime * speed;
    //     }
    //     if (keys[GLFW_KEY_S]) {
    //         pos -= dir * deltaTime * speed;
    //     }

    //     if (keys[GLFW_KEY_D]) {
    //         pos += right * deltaTime * speed;
    //     }
    //     if (keys[GLFW_KEY_A]) {
    //         pos -= right * deltaTime * speed;
    //     }
    //     if ((keys[GLFW_KEY_SPACE] && !isFall) || isJump) {
    //         jump(pos, deltaTime);
    //     }
    //     if(keys[GLFW_KEY_F5]) {
    //         viewPos = (viewPos == 2) ? 1 : (viewPos + 1);
    //     }

    //     fall(pos, deltaTime);

    //     // camera->setPosition(pos);

    //     std::vector<Entity*> entities = world->getChunk(glm::vec2(2))->entities;
    //     bool isBottomCollise = false;

    //     CollisionInfo info;

    //     for (auto const& entity : entities)
    //     {
    //         if(!entity->isCollisable()) {
    //             continue;
    //         }
    //         bool isCollides;

    //         CollisionBox* coll = new CollisionBox(pos, glm::vec3(1.f));
    //         isCollides = dynamic_cast<CollisionBox*>(entity)->checkCollision(coll, info);
            
    //         if(isCollides) {
    //             for (size_t i = 0; i < 3; i++)
    //             {
    //                 auto direction = pos - oldPos;

    //                 for (size_t i = 0; i < 3; i++)
    //                 {
    //                     if(fabs(info.direction[i]) != 0.1) {
    //                         if((info.direction[i] < 0 && direction[i] < 0) ||
    //                             (info.direction[i] > 0 && direction[i] > 0)) {
    //                             pos[i] = oldPos[i];
    //                         }
    //                     }
    //                 }

    //             }
                
    //         }

    //         if(!isBottomCollise && (!isCollides && info.direction.y < 0)) {
    //             isBottomCollise = true;
    //         }
    //     }

    //     isFall = !isBottomCollise;
        
    //     camera->setPosition(pos);

    //     // light->setPosition(camera->getPosition() + glm::vec3(10, 10, 0));
    // });


    camera->setViewCallback([&camera, &scene, &world](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {
        glm::vec3 pos = camera->getPosition();
        glm::vec3 oldPos = camera->getPosition();


        // printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", direction.x, direction.y, direction.z);
        auto op = direction.x * direction.x + direction.z * direction.z;
        // printf("[debag]: camera op: %f\n", op);
        glm::vec3 dir(std::clamp(direction.x / op, -1.f, 1.f), 0, std::clamp(direction.z / op, -1.f, 1.f));
        glm::vec3 rig(0,0,1);

        glm::vec3 upDir = glm::vec3(0, 2, 0);

        // printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", dir.x, dir.y, dir.z);

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

        
        camera->setPosition(pos);

        // light->setPosition(camera->getPosition() + glm::vec3(10, 10, 0));
    });

    ////////////////////////////////////////////////////


    // float blockProcess = 0.f;
    // Entity* selectEntity = NULL;
    // float cd = 0;
    // camera->setViewCallback([&camera, &headBlock, &scene, &blockProcess, &selectEntity, &cd](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {
    
    //     auto pos = camera->getPosition();

    //     auto entities = scene->getEntitiesWithinDistance(10);


    //     CollisionInfo info{};
    //     if(entities.size() != 0) {
    //         Entity* select = NULL;
    //         for (float i = 0; i <= 8.0f; i += 0.1f) {
    //             for (auto it = entities.begin(); it < entities.end(); it++) {
    //                 auto coll = new CollisionSphere((*it)->getPosition(), 1.f);
    //                 if(CollisionSphere(pos + (direction * i), .1f)
    //                     .checkCollision(coll, info) && (*it) != headBlock) {
    //                     select = (*it);
    //                     scene->selectEntity(select);
    //                     break;
    //                 };
    //                 delete coll;
    //             }
    //             if(select != NULL) break;
    //         }
            


    //             selectEntity = select;
    //             if(selectEntity == NULL) {
    //                 blockProcess = 0.f;
    //             }
    //     }

    //     // std::cout << mouse_keys[0] << std::endl;
    //     if(mouse_keys[GLFW_MOUSE_BUTTON_1]) {      
    //         if (selectEntity != NULL) {
    //             blockProcess += deltaTime;
    //         }
    //     }

    //     if(mouse_keys[GLFW_MOUSE_BUTTON_2] && cd <= 0) {      
    //         if (selectEntity != NULL) {
    //             glm::vec3 pos = selectEntity->getPosition();
    //             for (size_t i = 0; i < 3; i++){
    //                 if(info.direction[i] != 0) {
    //                     if(info.direction[i] > 0) {
    //                         pos[i] -= 2.f;
    //                     }else if(info.direction[i] < 0) {
    //                         pos[i] += 2.f;
    //                     }
    //                 }
    //             }
    //             bool isEmpty = true;

    //             for (auto const& entity : entities) {
    //                 if(entity->getPosition() == pos) {
    //                     isEmpty = false;
    //                     break;
    //                 }
    //             }
    //             if(isEmpty) {
    //                 Block* block = new Block(pos);
    //                 scene->addEntity(block);
    //             }
    //         }
    //         cd = .2;
            
    //     }
    //     if(cd > 0) {
    //         cd -= deltaTime;
    //     }

    //     if(blockProcess >= 1.f) {
    //         bool isBrokable = selectEntity->getType() == EntityType::BLOCK || selectEntity->getType() == EntityType::SLAB;

    //         if(isBrokable) {
    //             scene->removeEntity(selectEntity);
    //             blockProcess = 0.f;
    //             selectEntity = NULL;
    //         }
    //     }

    //     if(selectEntity == NULL) {
    //         blockProcess = 0.f;
    //     }
    //     if(selectEntity != NULL) {
    //         // std::cout << "Block Id: " << selectEntity->id << std::endl;
    //     }
        
    //     // std::cout << "Crash Process: " << blockProcess << std::endl;
    // });


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

    // glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
};


