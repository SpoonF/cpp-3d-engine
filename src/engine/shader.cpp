#include "engine/shader.h"

#include <iostream>

#include "utils/model.h"
#include "utils/imageBMP.h"
#include "engine/object.h"
#include "engine/camera.h"
#include <cstring>

GLFWwindow* Shader::window = nullptr;
std::shared_ptr<Camera> Shader::camera = nullptr;

char* Shader::readShader(const char* filename) {
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

void Shader::initShaderProgram(const char* vert, const char* frag) {
    if (glCreateProgram == nullptr) {
        printf("ОШИБКА: OpenGL функции не загружены! Забудьте вызвать gladLoadGL()\n");
        return;
    }

    glfwMakeContextCurrent(window);
    char* vertexShaderSource = readShader(vert);
    char* fragmentShaderSource = readShader(frag);

    if(strlen(vertexShaderSource) == 0) {
        printf("Ошибка вершинного шейдера: пустой файл шейдера");
    }

    if(strlen(fragmentShaderSource) == 0) {
        printf("Ошибка фрагментного шейдера: пустой файл шейдера");
    }

    // Создание шейдеров
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    delete[] vertexShaderSource;

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

    delete[] fragmentShaderSource;
    
    // Проверка компиляции фрагментного шейдера
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("Ошибка фрагментного шейдера: %s\n", infoLog);
    }
    
    // Создание шейдерной программы
    unsigned int shaderProgram = glCreateProgram();

    if (shaderProgram == 0) {
        printf("ОШИБКА: glCreateProgram вернул 0! OpenGL контекст не инициализирован\n");
        printf("Проверьте: glfwMakeContextCurrent и gladLoadGL вызваны ДО этого места\n");
        return;
    }
    
    printf("Создана шейдерная программа: %u\n", shaderProgram);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // Перед glLinkProgram:
    printf("Before glLinkProgram - shaderProgram=%u\n", shaderProgram);
    printf("OpenGL context: %p\n", glfwGetCurrentContext());

    // Проверка ошибок OpenGL перед линковкой
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        printf("OpenGL error before linking: 0x%x\n", err);
    }
    
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

    this->selectShader = shaderProgram;
}

void Shader::drawObjectInstaced(Model* model, imageBMP* texture, const ShaderOptions& options) {
    
    this->draw();

    unsigned int VAO, VBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);


    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, model->vertices.size() * sizeof(glm::vec3), &model->vertices[0], GL_STATIC_DRAW);

    // Позиции
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(0);

    if(options.positions.size()) {

        unsigned int positionsVBO;
        glGenBuffers(1, &positionsVBO);
        glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, positionsVBO);
        glBufferData(GL_ATOMIC_COUNTER_BUFFER, options.positions.size() * sizeof(glm::vec3), &options.positions[0], GL_DYNAMIC_DRAW);
        glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, positionsVBO);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(2, 1);  
    }

    if(options.sizes.size()) {

        unsigned int sizesVBO;
        glGenBuffers(1, &sizesVBO);
        glBindBuffer(GL_ARRAY_BUFFER, sizesVBO);
        glBufferData(GL_ARRAY_BUFFER, options.sizes.size() * sizeof(glm::vec3), &options.sizes[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, sizesVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(3, 1);  
    }

    unsigned int uvBuffer;
    glGenBuffers(1, &uvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glBufferData(GL_ARRAY_BUFFER, model->uvs.size() * sizeof(glm::vec2), &model->uvs[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    

    // uint hasTexture = glGetUniformLocation(1, "hasTexture");

    if(texture->data) {
        unsigned int textureLoc;
        glGenTextures(1, &textureLoc);
        glBindTexture(GL_TEXTURE_2D, textureLoc);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture->width, texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, texture->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
    }

    
    if(model->normals.size()) {
        unsigned int normal;
        glGenBuffers(1, &normal);
        glBindBuffer(GL_ARRAY_BUFFER, normal);
        glBufferData(GL_ARRAY_BUFFER, model->normals.size() * sizeof(glm::vec3), &model->normals[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glEnableVertexAttribArray(4);
        glBindBuffer(GL_ARRAY_BUFFER, normal);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, model->vertices.size(), options.positions.size());

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}



Shader::Shader(const char *vert, const char *frag)
{
    printf("Create shader");
    this->initShaderProgram(vert, frag);

    this->lastTime = glfwGetTime();
}

void Shader::draw() {

    // this->updateDeltaTime();

    glm::mat4 Projection = Shader::camera->getProjection();
    glm::mat4 View = Shader::camera->getView();
    glm::mat4 Model = getModel();
    glm::mat3 ModelNormal = glm::mat3(glm::transpose(glm::inverse(Model)));
    glm::vec3 viewPos = Shader::camera->getPosition();

    set("Projection", Projection);
    set("View", View);
    set("Model", Model);
    set("ModelNormal", ModelNormal);
    set("viewPos", viewPos);

    glUseProgram(this->selectShader);
}

void Shader::updateDeltaTime() {
    double currentTime = glfwGetTime();
    this->deltaTime = float(currentTime - this->lastTime);
    this->lastTime = glfwGetTime();
}
glm::mat4 Shader::getModel() {
    return glm::mat4(1.0f);
}
void Shader::set(const char* name, glm::vec3& data) {
    GLuint dataId = glGetUniformLocation(selectShader, name);
    glUniform3f(dataId, data.x, data.y, data.z);
}
void Shader::set(const char* name, int& data) {
    GLuint dataId = glGetUniformLocation(selectShader, name);

    glUniform1i(dataId, data);
}
void Shader::set(char* name, glm::mat4& data) {
    GLuint dataId = glGetUniformLocation(selectShader, name);

    glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
}
void Shader::set(char* name, glm::mat3& data) {
    GLuint dataId = glGetUniformLocation(selectShader, name);

    glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
}
void Shader::init(GLFWwindow* _window, std::shared_ptr<Camera> _camera)
{
    camera = _camera;
    std::cout << "Shader initialized with camera: " << camera << std::endl;
    
    window = _window;
    std::cout << "Shader initialized with window: " << window << std::endl;
}
Shader::~Shader()
{
    printf("Deleting shader %d\n", selectShader);
    if (selectShader != 0) {
        glDeleteProgram(selectShader);
    }
}

std::map<int, Shader*> Shader::instances;
Shader* Shader::getInstance(const char *vert, const char *frag, int id) {
    auto it = instances.find(id);
    if (it != instances.end() && it->second != nullptr) {
        return it->second;
    }
    
    Shader* newShader = new Shader(vert, frag);
    instances[id] = newShader;
    return newShader;
    
}

Shader *Shader::getInstance(int id) {
    auto it = instances.find(id);
    if (it != instances.end() && it->second != nullptr) {
        return it->second;
    }
    return nullptr;
}