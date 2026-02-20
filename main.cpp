#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <fstream>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtc/epsilon.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "engine.cpp"

char* readShader(const char* filename) {
    std::ifstream file(filename);

    assert(file.is_open());

    file.seekg(0, std::ios_base::end);
    size_t uSize = file.tellg();
    file.seekg(0);

    char* t = new char[uSize + 1];
    file.read(t, uSize);
    t[file.gcount()] = 0;

    file.close();

    return t;
}

int initShaderProgram() {
    char* vertexShaderSource = readShader("shaders/shader.vert");
    char* fragmentShaderSource = readShader("shaders/shader.frag");

    // Создание шейдеров
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    // Проверка компиляции вершинного шейдера
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("Ошибка вершинного шейдера: %s\n", infoLog);
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    // Проверка компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Ошибка фрагментного шейдера: %s\n", infoLog);
    }
    
    // Создание шейдерной программы
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    // Проверка линковки
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("Ошибка линковки: %s\n", infoLog);
    }
    
    // Удаление шейдеров
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Ошибка инициализации GLEW\n");
        return -1;
    }

    // unsigned int shaderProgram = initShaderProgram();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);


    Object3D obj2("./objects/cube.obj", "./objects/ground.bmp");

    Camera* camera = new Camera(window);

    Scene scene(window, camera);

    for (size_t x = 0; x < 16; x++)
    {
        for (size_t y = 0; y < 2; y++)
        {
            for (size_t z = 0; z < 16; z++)
            {
                if((y == 0 && std::rand() % 100 > 20) || std::rand() % 100 > 80) {
                    Entity* block = new Entity(obj2, glm::vec3(x*2 + 2, y*2, z*2 + 2));
                    // block->setPosition(glm::vec3(x*2 + 2, y*2, z*2 + 2));
                    scene.addEntity(block);
                }

                
            }
        }
    }
    
    float fallSpeed = 0;
    float speedFreeFall = 0.2;
    bool isFall = true;

    float lastTime;
    float moveSpeed = 0;
    float speed = 4;
    // bool isFly = true;
    float jumpProgress = 0;
    float jumpSpeed = 0.1f;
    bool isJump = false;


    // Где-то после создания окна

    float speedFall = .5f;

    auto jump = [&jumpProgress, &isJump, &isFall, &jumpSpeed, speedFreeFall](glm::vec3& pos, float deltaTime) {
        if(jumpProgress <= 2.f) {
            if(jumpSpeed > 0.f) {
                jumpSpeed -= 0.002 * deltaTime;
            }

            pos.y += jumpSpeed;
            jumpProgress += jumpSpeed;
            isJump = true;
            isFall = false;
        }else {
            jumpProgress = 0;
            jumpSpeed =  0.1f;
            isJump = false;
            isFall = true;
        }
        printf("%f", jumpProgress);

    };

    auto fall = [&isJump, &isFall, &fallSpeed, &speedFreeFall](glm::vec3& pos, float deltaTime) {
        if(isFall) {
            if(fallSpeed < 2.f) {
                fallSpeed += speedFreeFall * deltaTime;
            }

            pos.y -= fallSpeed;
        }else {
            fallSpeed = 0;
        }
    };



    // camera->setViewCallback([&lastTime, &moveSpeed, &camera, speed, &fallSpeed, speedFreeFall, &isFly, &scene, &isFalling, &isJump, &jumpProgress](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {

    //     glm::vec3 oldposition = camera->getPosition();
    //     glm::vec3 position = camera->getPosition();
    //     moveSpeed = speed;

    //     if (keys[GLFW_KEY_LEFT_CONTROL]){
    //         moveSpeed *= 2;
    //     }

    //     if (keys[GLFW_KEY_W]){
            
    //         position += direction * glm::vec3(1,0,1) * deltaTime * moveSpeed;
    //     }
    //     if (keys[GLFW_KEY_S]){
    //         position -= direction * glm::vec3(1,0,1) * deltaTime * moveSpeed;
    //     }

    //     if (keys[GLFW_KEY_D]){
    //         position += right * glm::vec3(1,0,1) * deltaTime * moveSpeed;
    //     }
    //     if (keys[GLFW_KEY_A]){
    //         position -= right * glm::vec3(1,0,1) * deltaTime * moveSpeed;
    //     }

    //     if (keys[GLFW_KEY_SPACE] && isFalling){
    //         position += glm::vec3(0, 2, 0) * deltaTime * moveSpeed;
    //     }
    //     if (keys[GLFW_KEY_LEFT_SHIFT] && isFalling){
    //         position -= glm::vec3(0, 2, 0) * deltaTime * moveSpeed;
    //     }


    //     if (keys[GLFW_KEY_SPACE] && !isFly && !isFalling){
    //         isJump = true;
    //     }  
    //     if (keys[GLFW_KEY_LEFT_ALT]){
    //         isFly = !isFly;
    //     }

    //     if(isFly == false && isJump == true) {
    //         if(fallSpeed <= 0.5) {
    //             fallSpeed += 2.4f * deltaTime;
    //         }
    //         if(jumpProgress <= 1) {
    //             position.y += fallSpeed;
    //             jumpProgress += fallSpeed;
    //         }else {
    //             isJump = false;
    //             isFalling = true;
    //         }
    //     }else {
    //         fallSpeed = 0.f;
    //     }
    //     if(isFly == false && isFalling == true) {
    //         if(fallSpeed <= 0.5) {
    //             fallSpeed += speedFreeFall * deltaTime;
    //         }
    //         if(position.y >= -200) {
    //             position.y -= fallSpeed;
    //         }
    //     }else {
    //         fallSpeed = 0.f;
    //     }

    //     // auto entities = scene.getEntities();

    //     // for (size_t i = 0; i < entities.size(); i++)
    //     // {
    //     //     if(camera->checkSphereCollision(entities[i]->getPosition())) {
    //     //         std::cout << "Пользователь Трогает: " << entities[i]->id << std::endl; 
    //     //         auto vec = oldposition - position;
    //     //         if(vec.y != 0.f) {
    //     //             isFalling = false;
    //     //             fallSpeed = 0.f;
    //     //         }
    //     //         position = camera->updatePosition(oldposition + (vec + vec));
    //     //     }
    //     // }
        

    //     camera->setPosition(position);
    // });

    Entity* headEntity = new Entity(Object3D("./objects/cube.obj", "./objects/ground.bmp"), glm::vec3(3, 13, 3));
    scene.addEntity(headEntity);
    camera->setViewCallback([&camera, &headEntity, speed, &scene, fall, jump, &isJump, &isFall](glm::vec3 direction, glm::vec3 right, glm::vec3 up, float deltaTime) {


        auto pos = headEntity->getPosition();
        auto oldPos = headEntity->getPosition();
        auto camPos = camera->getPosition();


        // printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", direction.x, direction.y, direction.z);
        auto op = direction.x * direction.x + direction.z * direction.z;
        printf("[debag]: camera op: %f\n", op);
        glm::vec3 dir(std::clamp(direction.x / op, -1.f, 1.f), 0, std::clamp(direction.z / op, -1.f, 1.f));
        glm::vec3 rig(0,0,1);

        printf("[debag]: camera dir data x: %f, y: %f, z: %f \n", dir.x, dir.y, dir.z);

        if (keys[GLFW_KEY_W]){
            pos += dir * deltaTime * speed;
        }
        if (keys[GLFW_KEY_S]){
            pos -= dir * deltaTime * speed;
        }

        if (keys[GLFW_KEY_D]){
            pos += right * deltaTime * speed;
        }
        if (keys[GLFW_KEY_A]){
            pos -= right * deltaTime * speed;
        }
        if ((keys[GLFW_KEY_SPACE] && !isFall) || isJump) {
            jump(pos, deltaTime);
        }

        fall(pos, deltaTime);

        headEntity->setPosition(pos);

        auto entities = scene.getEntities();
        bool isBottomCollise = false;

        for (auto it = entities.begin(); it != entities.end(); it++)
        {
            if((*it)->id != headEntity->id) {
                CollisionInfo info;

                bool isCollise = headEntity->checkSphereCollision((*it), info);
                
                if(isCollise) {
                    for (size_t i = 0; i < 3; i++)
                    {
                        auto direction = pos - oldPos;

                        for (size_t i = 0; i < 3; i++)
                        {
                            if(fabs(info.direction[i]) != 0) {
                                if((info.direction[i] < 0 && direction[i] < 0) ||
                                    (info.direction[i] > 0 && direction[i] > 0)) {
                                    pos[i] = oldPos[i];
                                }
                            }
                        }

                    }
                    
                }

                if(!isBottomCollise && (!isCollise && info.direction.y < 0)) {
                    isBottomCollise = true;
                }
                
            }
        }

        isFall = !isBottomCollise;
        
        headEntity->setPosition(pos);

        camPos = pos - direction * 10.f;
        camera->setPosition(camPos);

    });

    // Основной цикл
    while (isProgramRuning(window)) {
        // Очистка экрана
        glClearColor(0.529f, 0.808f, 0.922f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_CULL_FACE);

        scene.update();

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Очистка

    // glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
};

