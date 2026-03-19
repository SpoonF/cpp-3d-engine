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

#include "engine/entity/entity.cpp"

class Camera  {
    glm::vec3 position;
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float fov = 70.0f;
    float mouseSpeed = 0.05f;
    double xpos, ypos;
    int width, height;
    glm::vec3 direction;
    glm::vec3 right;
    glm::vec3 up;
    GLFWwindow* window;
    Object3D object;
    public: 
    using CallbackType = std::function<void( glm::vec3, glm::vec3, glm::vec3, float)>;
    Camera(GLFWwindow* window) {
        position = {0,0,0};
        this->window = window;
        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        object = Object3D("./objects/cube.obj");
    }
    glm::mat4 getView(float deltaTime) {
        glfwGetCursorPos(window, &xpos, &ypos);

        horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos);
        verticalAngle += mouseSpeed * deltaTime * float(height/2 - ypos);

        verticalAngle = std::min(verticalAngle, 1.5f);
        verticalAngle = std::max(verticalAngle, -1.5f);

        glm::vec3 direction(
            cos(verticalAngle) * sin(horizontalAngle),
            sin(verticalAngle),
            cos(verticalAngle) * cos(horizontalAngle)
        );

        glm::vec3 right(
            sin(horizontalAngle - 3.14f/2.0f),
            0,
            cos(horizontalAngle - 3.14f/2.0f)
        );

        glm::vec3 up = glm::cross( right, direction );

        for (size_t i = 0; i < callbacks_.size(); i++) {
            callbacks_[i](direction, right, up, deltaTime);
        }

        glfwSetCursorPos(window, width/2, height/2);

        return glm::lookAt(
            position,
            position + direction,
            up
        );
    }
    glm::mat4 getProjection() {
        return glm::perspective(glm::radians(fov), 4.f/3.f, 0.1f, 100.0f);
    }
    void setViewCallback(CallbackType action) {
        callbacks_.push_back(action);
    }

    void setPosition(const glm::vec3& pos)  {
        this->position = pos;
    }
    glm::vec3 getPosition()  {
        return this->position;
    }
    int getVertexCount()  {
        return object.model->vertices.size();
    }
    glm::vec3 getVertex(int i)  {
        return object.model->vertices[i];
    }
private:
    std::vector<CallbackType> callbacks_;
};

class Shader {
    GLFWwindow *window;
    Camera* camera;

    int selectShader;
    std::map<const char*, int> instances;
    double lastTime;
    double deltaTime;
    std::queue<std::function<void()>> drawQueue = std::queue<std::function<void()>>();
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

    int initShaderProgram(const char* vert, const char* frag) {
        char* vertexShaderSource = readShader(vert);
        char* fragmentShaderSource = readShader(frag);

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
    void drawObject(const Object3D& obj) {
        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);
        
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, obj.model->vertices.size() * sizeof(glm::vec3), &obj.model->vertices[0], GL_STATIC_DRAW);
        
        // Позиции
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        GLuint uvBuffer;
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, obj.model->uvs.size() * sizeof(glm::vec2), &obj.model->uvs[0], GL_STATIC_DRAW);

        GLuint normalBuffer;
        glGenBuffers(2, &normalBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glBufferData(GL_ARRAY_BUFFER, obj.model->normals.size() * sizeof(glm::vec2), &obj.model->normals[0], GL_STATIC_DRAW);


        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, obj.texture->width, obj.texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, obj.texture->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Цвета

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, obj.model->vertices.size());

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    void drawObjectInstaced(const Object3D& obj, const std::vector<glm::vec3>& instances) {
        unsigned int VAO, VBO;

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);


        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, obj.model->vertices.size() * sizeof(glm::vec3), &obj.model->vertices[0], GL_STATIC_DRAW);

        // Позиции
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        glEnableVertexAttribArray(0);

        unsigned int instanceVBO;
        glGenBuffers(1, &instanceVBO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, instances.size() * sizeof(glm::vec3), &instances[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        unsigned int uvBuffer;
        glGenBuffers(1, &uvBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glBufferData(GL_ARRAY_BUFFER, obj.model->uvs.size() * sizeof(glm::vec2), &obj.model->uvs[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, obj.texture->width, obj.texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, obj.texture->data);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);
        
        
        unsigned int normal;
        glGenBuffers(1, &normal);
        glBindBuffer(GL_ARRAY_BUFFER, normal);
        glBufferData(GL_ARRAY_BUFFER, obj.model->normals.size() * sizeof(glm::vec3), &obj.model->normals[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        
        // Цвета

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(2, 1);    


        glEnableVertexAttribArray(3);
        glBindBuffer(GL_ARRAY_BUFFER, normal);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);


        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, obj.model->vertices.size(), instances.size());

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    public:
    Shader(GLFWwindow *window, Camera* camera) {
        this->camera = camera;
        this->window = window;

        instances.emplace("default", initShaderProgram("shaders/shader.vert", "shaders/shader.frag"));
        selectShader = instances.at("default");

        lastTime = glfwGetTime();
    }

    void add(const char* name, const char* vert, const char* frag) {
        instances.emplace(name, initShaderProgram(vert, frag));
    }
    void useShdr(const char* name) {
        selectShader = instances.at(name);
    }
    void draw() {

        glm::mat4 Projection = camera->getProjection();
        glm::mat4 View = camera->getView(deltaTime);
        glm::mat4 Model = getModel();
        glm::mat3 ModelNormal = glm::mat3(glm::transpose(glm::inverse(Model)));
        glm::vec3 viewPos = camera->getPosition();

        set("Projection", Projection);
        set("View", View);
        set("Model", Model);
        set("ModelNormal", ModelNormal);
        set("viewPos", viewPos);

        glUseProgram(selectShader);

        while (!drawQueue.empty())
        {
            drawQueue.front()();
            drawQueue.pop();
        }
        // drawCrossPoint(shaderId);
        
    }
    void updateDeltaTime() {
        double currentTime = glfwGetTime();
        deltaTime = float(currentTime - lastTime);
        lastTime = glfwGetTime();
    }
    glm::mat4 getModel() {
        return glm::mat4(1.0f);
    }
    void set(const char* name, glm::vec3& data) {
        GLuint dataId = glGetUniformLocation(selectShader, name);
        glUniform3f(dataId, data.x, data.y, data.z);
    }
    void set(const char* name, int& data) {
        GLuint dataId = glGetUniformLocation(selectShader, name);

        glUniform1i(dataId, data);
    }
    void set(char* name, glm::mat4& data) {
        GLuint dataId = glGetUniformLocation(selectShader, name);

        glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
    }
    void set(char* name, glm::mat3& data) {
        GLuint dataId = glGetUniformLocation(selectShader, name);

        glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
    }
    void addDraw(const Object3D& object){
        drawQueue.push([this, object]() { drawObject(object); });
    }
    void addDraw(const Object3D& object, const std::vector<glm::vec3>& instances){
        drawQueue.push([this, object, instances]() { drawObjectInstaced(object, instances); });
    }
};

class EntityCompentities {
private:
    Object3D object;
public:
    std::vector<glm::vec3> positions;
    EntityCompentities(const Object3D& object) {
        this->object = object;
    }
    EntityCompentities(const Object3D& object, glm::vec3 position) {
        this->object = object;
        this->addPosition(position);
    }
    void addPosition(glm::vec3& position) {
        this->positions.push_back(position);
    };
    std::vector<glm::vec3> getPositions() {
        return this->positions;
    }
    Object3D getObject() {
        return this->object;
    };
};

class Scene {
    GLFWwindow* window;
    Shader* shdr;
    Camera* camera;
    public:
    std::vector<std::vector<int>> map;
    std::vector<Entity*> entities;
    std::vector<Entity*> selectedEntities;
    std::vector<int> transperents;
    Scene(GLFWwindow *window, Camera* camera) {
        this->window = window;
        shdr = new Shader(window, camera);
        shdr->add("select", "shaders/shader.vert", "shaders/shaderOneColor.frag");
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

        std::vector<EntityCompentities> vec;
        shdr->updateDeltaTime();

        for (auto const &entity : entities)
        {
            // if(entities[i]->isSelected) {
            //    continue;
            // }




            Object3D object = entity->getObject();
            glm::vec3 position = entity->getPosition();

            if(entity->getType() == EntityType::LIGHT) {
                shdr->set("lightPos", position);
            }

            size_t j = 0;

            auto it = std::find_if(vec.begin(), vec.end(), [&object](EntityCompentities ec) {
                return ec.getObject() == object;
            });

            if(it != vec.end()) {
                vec.at(j).positions.push_back(position);
            }else {
                vec.push_back(EntityCompentities(object, position));
            }
           
        }

        for (size_t i = 0; i < vec.size(); i++)
        {
            shdr->addDraw(vec[i].getObject(), vec[i].getPositions());
        }

        shdr->draw();
        
    }
};
