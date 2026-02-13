#include <cassert>
#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
#include <queue>
#include <functional>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class imageBMP {
public:
    unsigned char header[54];
    unsigned int dataPos;
    unsigned int width, height;
    unsigned int size;
    unsigned char* data;
    imageBMP(const char* filename) {
        FILE* file = fopen(filename, "rb");
        
        if (!file) {
            printf("Изображение не может быть открытоn");
            exit(0);
        }
        
        if( fread(header, 1, 54, file) != 54 ) {
            printf("Некорректный BMP-файлn");
            exit(0);
        }

        if(header[0] != 'B' && header[1] != 'M') {
            printf("Некорректный BMP-файлn");
            exit(0);
        }

        dataPos = *(int*)&(header[0x0A]);
        size = *(int*)&(header[0x22]);
        width = *(int*)&(header[0x12]);
        height = *(int*)&(header[0x16]);

        if(size == 0) size = width * height * 3;
        if(dataPos == 0) dataPos = 54;

        data = new unsigned char [size];
        fread(data, 1, size, file);
        fclose(file);
    }
};

class Model {
    public:
    vector<glm::vec3> vertices;
    vector<glm::vec2> uvs;
    vector<glm::vec3> normals;
    Model(const char* filename) {
        FILE* file = fopen(filename, "r");

        assert(file != NULL && "Impossible to open the file !\n");

        vector<glm::vec3> tmp_vertices;
        vector<glm::vec2> tmp_uvs;
        vector<glm::vec3> tmp_normals;
        vector<int> fragVert;
        vector<int> fragUV;
        vector<int> fragNorm;

        while (true)
        {
            char lineHeader[128];

            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF) break;

            if ( strcmp(lineHeader, "v") == 0 ) {
                glm::vec3 vertex; 
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
                tmp_vertices.push_back(vertex);
            } else if ( strcmp(lineHeader, "vt") == 0 ) {
                glm::vec2 uv;
                fscanf(file, "%f %f\n", &uv.x, &uv.y);
                tmp_uvs.push_back(uv);
            } else if ( strcmp(lineHeader, "vn") == 0 ) {
                glm::vec3 normal;
                fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
                tmp_normals.push_back(normal);
            } else if ( strcmp(lineHeader, "f") == 0 ) {
                string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                
                assert(matches == 9 && "Fuck!");

                
                fragVert.push_back(vertexIndex[0]);
                fragVert.push_back(vertexIndex[1]);
                fragVert.push_back(vertexIndex[2]);
                fragUV.push_back(uvIndex[0]);
                fragUV.push_back(uvIndex[1]);
                fragUV.push_back(uvIndex[2]);
                fragNorm.push_back(normalIndex[0]);
                fragNorm.push_back(normalIndex[1]);
                fragNorm.push_back(normalIndex[2]);
            }
        }

        for (size_t i = 0; i < fragVert.size(); i++)
        {
            vertices.push_back(tmp_vertices[ fragVert[i] - 1 ]);
        }

        for (size_t i = 0; i < fragUV.size(); i++)
        {
            uvs.push_back(tmp_uvs[ fragUV[i] - 1 ]);
        }

        for (size_t i = 0; i < fragNorm.size(); i++)
        {
            normals.push_back(tmp_normals[ fragNorm[i] - 1 ]);
        }
    }
};



class Texture {
    public:
    Texture(const char* filename) {
        FILE* file = fopen(filename, "rb");
        
    }
};

class Object {
    
};

class Object3D {
    public:
    Model* model;
    imageBMP* texture;
    Object3D() {}
    Object3D(Model* _model, imageBMP* _texture) {
        model = _model;
        texture = _texture;
    }
    Object3D(const char* modelpath, const char* texturepath) {
        model = new Model(modelpath);
        texture = new imageBMP(texturepath);;
    }
    Object3D(const char* modelpath) {
        model = new Model(modelpath);
    }
};
class Entity {
    Object3D _object;
    glm::vec3 _pos = glm::vec3(0.0f, 0.0f, 0.0f);
    public:
    Entity(Object3D obj) {
        _object = obj;
    }
    void setPosition(glm::vec3 pos) {
        _pos = pos;
    }
    Object3D getObject() {
        return _object;
    }
    glm::vec3 getPosition() {
        return _pos;
    }
};


class Player {
    glm::vec3 position = glm::vec3(0, 0, 0);
    public:
    Player(){}
    void setPosition(glm::vec3 pos) {
        position = pos;
    }
    glm::vec3 getPosition(glm::vec3 pos) {
        return position;
    }
};

class Shader {
    GLFWwindow *window;
    int shaderId;
    glm::vec3 position = glm::vec3(0, 0, 5);
    float horizontalAngle = 3.14f;
    float verticalAngle = 0.0f;
    float fov = 45.0f;

    float speed = 3.f;
    float mouseSpeed = 0.05f;

    double xpos, ypos;
    int width, height;
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
    void drawObject(Object3D obj) {
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
    void drawObjectInstaced(Object3D obj, std::vector<glm::vec3> instances) {
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
        
        GLuint uvBuffer;
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

        // glEnableVertexAttribArray(2);
        // glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(2, 1);   
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        glBindVertexArray(VAO);
        // glDrawArrays(GL_TRIANGLES, 0, object.model->vertices.size());
        glDrawArraysInstanced(GL_TRIANGLES, 0, obj.model->vertices.size(), instances.size());

        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
    }
    public:
    Shader(GLFWwindow *_window) {
        shaderId = initShaderProgram();
        window = _window;

        glfwGetCursorPos(window, &xpos, &ypos);
        glfwGetWindowSize(window, &width, &height);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        lastTime = glfwGetTime();
    }
    void draw() {
        glfwGetCursorPos(window, &xpos, &ypos);

        double currentTime = glfwGetTime();
        float deltaTime = float(currentTime - lastTime);
        lastTime = glfwGetTime();

        horizontalAngle += mouseSpeed * deltaTime * float(width/2 - xpos);
        verticalAngle += mouseSpeed * deltaTime * float(height/2 - ypos);

        glm::mat4 Projection = getProjection();
        glm::mat4 View = getView(deltaTime);
        glm::mat4 Model = getModel();

        glm::mat4 MVP = Projection * View * Model;

        set("MVP", MVP);


        glUseProgram(shaderId);

        while (!drawQueue.empty())
        {
            drawQueue.front()();
            drawQueue.pop();
        }

        glfwSetCursorPos(window, width/2, height/2);
    }
    glm::mat4 getProjection() {
        return glm::perspective(glm::radians(fov), 4.f/3.f, 0.1f, 100.0f);
    }
    glm::mat4 getView(float deltaTime) {
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

        glm::vec3 upDir(
            0,
            2.f,
            0
        );

        glm::vec3 up = glm::cross( right, direction );

        float moveSpeed = speed;

        if (glfwGetKey(window,  GLFW_KEY_LEFT_CONTROL ) == GLFW_PRESS){
            moveSpeed *= 2;
        }

        if (glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS){
            position += direction * deltaTime * moveSpeed;
        }
        // Движение назад
        if (glfwGetKey(window,  GLFW_KEY_S ) == GLFW_PRESS){
            position -= direction * deltaTime * moveSpeed;
        }
        // Стрэйф вправо
        if (glfwGetKey(window,  GLFW_KEY_D ) == GLFW_PRESS){
            position += right * deltaTime * moveSpeed;
        }
        // Стрэйф влево
        if (glfwGetKey(window,  GLFW_KEY_A ) == GLFW_PRESS){
            position -= right * deltaTime * moveSpeed;
        }

        // Движение вверх
        if (glfwGetKey(window,  GLFW_KEY_SPACE ) == GLFW_PRESS){
            position += upDir * deltaTime * moveSpeed;
        }
        // Движение вниз
        if (glfwGetKey(window,  GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS){
            position -= upDir * deltaTime * moveSpeed;
        }

        return glm::lookAt(
            position,
            position + direction,
            up
        );
    }
    glm::mat4 getModel() {
        return glm::mat4(1.0f);
    }
    void set(const char* name, glm::vec3 data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniform3f(dataId, data.x, data.y, data.z);
    }
    void set(const char* name, int data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniform1i(dataId, data);
    }
    void set(char* name, glm::mat4 data) {
        GLuint dataId = glGetUniformLocation(shaderId, name);

        glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
    }
    void addDraw(Object3D object){
        drawQueue.push([this, object]() { drawObject(object); });
    }
    void addDraw(Object3D object, std::vector<glm::vec3> instances){
        drawQueue.push([this, object, instances]() { drawObjectInstaced(object, instances); });
    }
};

class Scene {
    GLFWwindow *_window;
    Shader* shdr;
    public:
    vector<vector<int>> map;
    std::vector<Entity> entities;
    Scene(GLFWwindow *window) {
        _window = window;
        shdr = new Shader(window);
    }
    void addEntity(Entity entity) {
        entities.push_back(entity);
    }
    void update() {

        Object3D object = entities[0].getObject();

        std::vector<glm::vec3> positions;
        for (size_t i = 0; i < entities.size(); i++)
        {
            positions.push_back(entities[i].getPosition());
        }

        shdr->addDraw(object, positions);
        shdr->draw();
        
    }

    void setShader(int shaderProgram, const char* name, glm::vec3 data) {
        GLuint dataId = glGetUniformLocation(shaderProgram, name);

        glUniform3f(dataId, data.x, data.y, data.z);
    }
    void setShader(int shaderProgram, const char* name, int data) {
        GLuint dataId = glGetUniformLocation(shaderProgram, name);

        glUniform1i(dataId, data);
    }
    void setShader(int shaderProgram, char* name, glm::mat4 data) {
        GLuint dataId = glGetUniformLocation(shaderProgram, name);

        glUniformMatrix4fv(dataId, 1, GL_FALSE, &data[0][0]);
    }
    void drawObject(unsigned int& VAO, unsigned int& VBO, Object3D obj) {

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
    }
    void drawObjectInstaced(unsigned int& VAO, unsigned int& VBO, Object3D obj, std::vector<glm::vec3> positions) {
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
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), &positions[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        
        GLuint uvBuffer;
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

        // glEnableVertexAttribArray(2);
        // glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
        // glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);	
        glVertexAttribDivisor(2, 1);   
        
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
};
