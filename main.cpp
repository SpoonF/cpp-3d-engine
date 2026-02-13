#include <stdio.h>
#include <stdlib.h>
#include <cstdlib> 
#include <iostream>
#include <fstream>
#include "engine.cpp"

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Ошибка инициализации GLEW\n");
        return -1;
    }

    // unsigned int shaderProgram = initShaderProgram();


    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    Object3D obj("./objects/head.obj", "./objects/head.bmp");
    Object3D obj2("./objects/cube.obj", "./objects/ground.bmp");

    Player player1;

    Scene scene(window);
    // scene.setPlayer(player1);
    Entity en(obj);
    en.setPosition(glm::vec3(0.f, 0,0));
    Entity blockEntity(obj2);
    en.setPosition(glm::vec3(0.f, 0,0));

    for (size_t x = 0; x < 64; x++)
    {
        for (size_t y = 0; y < 1; y++)
        {
            for (size_t z = 0; z < 64; z++)
            {
                Entity block{blockEntity};
                block.setPosition(glm::vec3(x*2, y*2, z*2));
                scene.addEntity(block);
            }
        }
    }
    

    // Основной цикл
    while (isProgramRuning(window)) {
        // Очистка экрана
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
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
}