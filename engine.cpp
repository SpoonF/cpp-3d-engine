#include <cassert>
#include <iostream>
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

#include "engine/object.cpp"

class CollisionInfo {
public:
    glm::vec3 Normal;
    float PenetrationDepth;
    std::vector<glm::vec3> collisionPoints;
    glm::vec3 direction;
};

class Plane {
public:
    glm::vec3 P1;
    glm::vec3 Normal;

    inline Plane(glm::vec3 p1, glm::vec3 normal) {
        P1 = p1;
        Normal = normal;
    }
};

class Collision {
public:
    glm::vec3 position;
    Object3D object;
    float radius = 1.15f;
    Collision(const Object3D& object, glm::vec3 position) {
        this->object = object;
        this->position = position;
    }
    Collision(glm::vec3 position) {
        this->position = position;
    }

    virtual int getVertexCount() {
        return object.model->vertices.size();
    };
    virtual glm::vec3 getVertex(int i) {
        return object.model->vertices[i];
    };
    virtual void setPosition(const glm::vec3& pos) {
        this->position = pos;
    };
    virtual glm::vec3 getPosition() {
        return position;
    };
    bool checkSphereCollision(Collision* other, CollisionInfo& info, float radius = NULL) {
        auto getDirection = [](glm::vec3 delta){
            glm::vec3 direction{0, 0, 0};
            size_t bigIndex = 0;
            for (size_t i = 1; i < 3; i++)
            {
                if(fabs(delta[bigIndex]) < fabs(delta[i])) {
                    bigIndex = i;
                }
            }
            direction[bigIndex] = delta[bigIndex];

            return direction;
        };

        glm::vec3 delta = other->getPosition() - this->getPosition();

        if(radius == NULL) {
            radius = this->radius;
        }
        
        float distance = glm::length(delta);
        float radiusSum = radius + other->radius;
        
        if (distance < radiusSum) {
            // Есть коллизия
            info.Normal = glm::normalize(delta);
            info.PenetrationDepth = radiusSum - distance;
            info.direction = getDirection(delta);
            info.collisionPoints.push_back(
                this->getPosition() + info.Normal
            );

            // printf("[debag]: direction data x: %f, y: %f, z: %f \n", info.direction.x, info.direction.y, info.direction.z);
            return true;
        }
        return false;
    }

    bool checkBoxCollision(Collision* other, CollisionInfo& info) {
        auto getDirection = [](glm::vec3 delta){
            glm::vec3 direction{0, 0, 0};
            size_t bigIndex = 0;
            for (size_t i = 1; i < 3; i++)
            {
                if(fabs(delta[bigIndex]) < fabs(delta[i])) {
                    bigIndex = i;
                }
            }
            direction[bigIndex] = delta[bigIndex];

            return direction;
        };

        glm::vec3 pos = this->getPosition();
        glm::vec3 oPos = other->getPosition();

        glm::vec3 delta = oPos - pos;
        
        bool collision = 
            (oPos.x <= (pos.x + 2.0f) && (oPos.x + 2.0f) >= pos.x) &&
            (oPos.y <= (pos.y + 2.0f) && (oPos.y + 2.0f) >= pos.y) &&
            (oPos.z <= (pos.z + 2.0f) && (oPos.z + 2.0f) >= pos.z);
        
        if (collision) {
            // Есть коллизия
            info.Normal = glm::normalize(delta);
            // info.PenetrationDepth = radiusSum - distance;
            info.direction = getDirection(delta);
            info.collisionPoints.push_back(
                this->getPosition() + info.Normal
            );

            // printf("[debag]: direction data x: %f, y: %f, z: %f \n", info.direction.x, info.direction.y, info.direction.z);
            return true;
        }
        return false;
    }
};

enum EntityType {
    BLOCK = 0,
};

class Entity: public Collision {

public:
    static inline int count = 0;
    glm::vec3 rotate{0,0,0};
    EntityType type;
    int id = 0;
    Entity(const Object3D& object, glm::vec3 position, EntityType type): Collision(object, position) {
        count++;
        id = count;
        this->type = type;
    }
    bool constains(const glm::vec3& point) const {
        return glm::all(glm::greaterThanEqual(point, position)) && 
        glm::all(glm::lessThanEqual(point, position + 1.f));
    }
    void setRotate(glm::vec3 rotate) {
        this->rotate = rotate;
    }
    glm::vec3 getRotate() {
        return rotate;
    }
    Object3D getObject() {
        return object;
    }
    EntityType getType() {
        return type;
    }
    
    // void setPosition(const glm::vec3& pos)  {
    //     this->position = pos;
    // }
    // glm::vec3 getPosition()  {
    //     return this->position;
    // }
    // int getVertexCount()  {
    //     return object.model->vertices.size();
    // }
    // glm::vec3 getVertex(int i)  {
    //     return object.model->vertices[i];
    // }
};
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
    glm::mat4 getView(float& deltaTime) {
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
        return glm::perspective(glm::radians(fov), 4.f/3.f, 0.1f, 300.0f);
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

    int shaderId;
    double lastTime;
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
    void drawObjectInstaced(const Object3D& obj, const std::vector<glm::vec3>& instances, const std::vector<glm::mat4>& rotations) {
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

        unsigned int rotationVBO;
        glGenBuffers(1, &rotationVBO);
        glBindBuffer(GL_ARRAY_BUFFER, rotationVBO);
        glBufferData(GL_ARRAY_BUFFER, rotations.size() * sizeof(glm::mat4), &rotations[0], GL_STATIC_DRAW);
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
        glBindBuffer(GL_ARRAY_BUFFER, rotationVBO);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(3, 1);   
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, object.model->vertices.size());
        glDrawArraysInstanced(GL_TRIANGLES, 0, obj.model->vertices.size(), instances.size());

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    public:
    Shader(GLFWwindow *window, Camera* camera) {
        this->camera = camera;
        this->window = window;


        shaderId = initShaderProgram();
        lastTime = glfwGetTime();
    }
    void draw() {

        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = glfwGetTime();

        glm::mat4 Projection = camera->getProjection();
        glm::mat4 View = camera->getView(deltaTime);
        glm::mat4 Model = getModel();

        set("Projection", Projection);
        set("View", View);
        set("Model", Model);

        glUseProgram(shaderId);

        while (!drawQueue.empty())
        {
            drawQueue.front()();
            drawQueue.pop();
        }
        // drawCrossPoint(shaderId);
        
    }
    glm::mat4 getModel() {
        return glm::mat4(1.0f);
    }
    void set(const char* name, glm::vec3& data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniform3f(dataId, data.x, data.y, data.z);
    }
    void set(const char* name, int& data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniform1i(dataId, data);
    }
    void set(char* name, glm::mat4& data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
    }
    void addDraw(const Object3D& object){
        drawQueue.push([this, object]() { drawObject(object); });
    }
    void addDraw(const Object3D& object, const std::vector<glm::vec3>& instances, const std::vector<glm::mat4>& rotations){
        drawQueue.push([this, object, instances, rotations]() { drawObjectInstaced(object, instances, rotations); });
    }
    // void drawCrossPoint(int shaderProgram) {
    //     float size = 0.1f;
    //     GLfloat vertices[] = {
    //         // Горизонтальная линия (слева направо)
    //         -size, 0.0f,  // Левая точка
    //         size, 0.0f,  // Правая точка

    //         // Вертикальная линия (снизу вверх)
    //         0.0f, -size,  // Нижняя точка
    //         0.0f,  size   // Верхняя точка
    //     };

    //     GLuint VAO, VBO;

    //     // Создаем и связываем VAO
    //     glGenVertexArrays(1, &VAO);
    //     glGenBuffers(1, &VBO);

    //     glBindVertexArray(VAO);

    //     // Загружаем данные в VBO
    //     glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //     // Настраиваем атрибуты вершин (позиция)
    //     // layout (location = 0) in vec2 aPos;
    //     glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
    //     glEnableVertexAttribArray(0);

    //     // Отвязываем (необязательно)
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);
    //     glBindVertexArray(0);

    //     glDisable(GL_DEPTH_TEST);

    //     // Если нужен цвет
    //     GLint colorLoc = glGetUniformLocation(shaderProgram, "uColor");
    //     glUniform3f(colorLoc, 1.0f, 1.0f, 1.0f); // Белый

    //     // Рисуем
    //     glBindVertexArray(VAO);

    //     // glDrawArrays(режим, начальный индекс, количество вершин)
    //     // GL_LINES будет рисовать пары вершин: (0,1) и (2,3)
    //     glDrawArrays(GL_LINES, 0, 4); // 4 вершины = 2 линии

    //     glBindVertexArray(0);

    //     // Включаем тест глубины обратно
    //     glEnable(GL_DEPTH_TEST);
    // }
};

class Scene {
    GLFWwindow* window;
    Shader* shdr;
    Camera* camera;
    public:
    std::vector<std::vector<int>> map;
    std::vector<Entity*> entities;
    Scene(GLFWwindow *window, Camera* camera) {
        this->window = window;
        shdr = new Shader(window, camera);
    }
    void addEntity(Entity* entity) {
        entities.push_back(entity);
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

        std::map<Object3D, std::tuple<std::vector<glm::vec3>, std::vector<glm::mat4>>> map;

        for (size_t i = 0; i < entities.size(); i++)
        {
            Object3D object = entities[i]->getObject();
            glm::vec3 position = entities[i]->getPosition();
            glm::vec3 rotate = entities[i]->getRotate();

            if(map.count(object) == 1) {
                std::get<0>(map.at(object)).push_back(position);
                std::get<1>(map.at(object)).push_back(glm::mat4_cast(glm::rotation(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    rotate 
                )));
            }else {
                std::vector<glm::vec3> tmpPos;
                std::vector<glm::mat4> tmpRot;
                tmpPos.push_back(position);
                tmpRot.push_back(glm::mat4_cast(glm::rotation(
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    rotate 
                )));
                map.insert({object, {tmpPos, tmpRot}});
            }
        }

        for (auto it = map.begin(); it != map.end(); it++)
        {
            shdr->addDraw(it->first, std::get<0>(it->second), std::get<1>(it->second));
        }
        
        shdr->draw();
        
    }
};
