#include "engine/shader.h"
#include "engine/object.h"
#include "engine/camera.h"
#include "utils/model.h"
#include "utils/imageBMP.h"

Camera* Shader::camera = nullptr;
GLFWwindow* Shader::window = nullptr;

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

    this->selectShader = shaderProgram;
}

void Shader::drawObjectInstaced(const Object3D& obj, const ShaderOptions& options) {
    
    this->draw();

    // unsigned int VAO, VBO;

    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);


    // glBindVertexArray(VAO);
    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, obj.model->vertices.size() * sizeof(glm::vec3), &obj.model->vertices[0], GL_STATIC_DRAW);

    // // Позиции
    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    // glEnableVertexAttribArray(0);

    // if(options.positions.size()) {
    //     unsigned int positionsVBO;
    //     glGenBuffers(1, &positionsVBO);
    //     glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    //     glBufferData(GL_ARRAY_BUFFER, options.positions.size() * sizeof(glm::vec3), &options.positions[0], GL_STATIC_DRAW);
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);

    //     glEnableVertexAttribArray(2);
    //     glBindBuffer(GL_ARRAY_BUFFER, positionsVBO);
    //     glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //     glBindBuffer(GL_ARRAY_BUFFER, 0);	
    //     glVertexAttribDivisor(2, 1);  
    // }

    
    
    // unsigned int uvBuffer;
    // glGenBuffers(1, &uvBuffer);
    // glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    // glBufferData(GL_ARRAY_BUFFER, obj.model->uvs.size() * sizeof(glm::vec2), &obj.model->uvs[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unsigned int texture;
    // glGenTextures(1, &texture);
    // glBindTexture(GL_TEXTURE_2D, texture);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, obj.texture->width, obj.texture->height, 0, GL_BGR, GL_UNSIGNED_BYTE, obj.texture->data);

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    // glGenerateMipmap(GL_TEXTURE_2D);
    
    
    // unsigned int normal;
    // glGenBuffers(1, &normal);
    // glBindBuffer(GL_ARRAY_BUFFER, normal);
    // glBufferData(GL_ARRAY_BUFFER, obj.model->normals.size() * sizeof(glm::vec3), &obj.model->normals[0], GL_STATIC_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    
    // // Цвета

    // glEnableVertexAttribArray(1);
    // glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
    // glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

  


    // glEnableVertexAttribArray(3);
    // glBindBuffer(GL_ARRAY_BUFFER, normal);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (GLvoid*)0);


    
    // glBindBuffer(GL_ARRAY_BUFFER, 0);
    // glBindVertexArray(0);

    // glBindVertexArray(VAO);
    // glDrawArraysInstanced(GL_TRIANGLES, 0, obj.model->vertices.size(), options.positions.size());

    // glDeleteVertexArrays(1, &VAO);
    // glDeleteBuffers(1, &VBO);
}

std::unique_ptr<Shader> Shader::create(const char *vert, const char *frag)
{
    auto instance = std::unique_ptr<Shader>();
    instance->initShaderProgram(vert, frag);
    return instance;
}

Shader::Shader(const char *vert, const char *frag)
{
    this->initShaderProgram(vert, frag);
}

void Shader::draw() {

    glm::mat4 Projection = Shader::camera->getProjection();
    glm::mat4 View = Shader::camera->getView(this->deltaTime);
    glm::mat4 Model = getModel();
    glm::mat3 ModelNormal = glm::mat3(glm::transpose(glm::inverse(Model)));
    glm::vec3 viewPos = Shader::camera->getPosition();

    set("Projection", Projection);
    set("View", View);
    set("Model", Model);
    set("ModelNormal", ModelNormal);
    set("viewPos", viewPos);

    // glUseProgram(this->selectShader);

    // while (!this->drawQueue.empty())
    // {
    //     this->drawQueue.front()();
    //     this->drawQueue.pop();
    // }
    // drawCrossPoint(shaderId);
    
}
void Shader::updateDeltaTime() {
    double currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);
    lastTime = glfwGetTime();
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
void Shader::init(GLFWwindow* _window, Camera* _camera)
{
    
    camera = _camera;
    window = _window;

    std::cout << "Shader initialized with camera: " << camera << std::endl;

}