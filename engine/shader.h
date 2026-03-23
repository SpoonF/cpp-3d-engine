#ifndef SHADER_H
#define SHADER_H

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi
#include <glm/gtx/quaternion.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <map>
#include <queue>
#include <fstream>

#include <camera.h>
#include <object.h>

struct ShaderOptions {
    const std::vector<glm::vec3>& positions;
};

class Shader {
    GLFWwindow *window;
    Camera* camera;

    int selectShader;
    double lastTime;
    double deltaTime;
    std::queue<std::function<void()>> drawQueue = std::queue<std::function<void()>>();
    char* readShader(const char* filename);

    public:
    int initShaderProgram(const char* vert, const char* frag);
    void drawObjectInstaced(const Object3D& obj);
    Shader(GLFWwindow *window, Camera* camera);
    void draw();
    void updateDeltaTime();
    glm::mat4 getModel();
    void set(const char* name, glm::vec3& data);
    void set(const char* name, int& data);
    void set(char* name, glm::mat4& data);
    void set(char* name, glm::mat3& data);
    void addDraw(const Object3D& object);
    void addDraw(const Object3D& object, const std::vector<glm::vec3>& instances);
    void addDraw(const Object3D& object);
};

#endif

