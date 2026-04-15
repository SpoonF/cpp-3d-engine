#pragma once

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
#include <set>
#include <queue>
#include <fstream>
#include <functional>
#include <memory>

#include "object.h"
#include "camera.h"
#include "frustum.h"

struct ShaderOptions {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> sizes;
    
};

class Shader {
    static GLFWwindow* window;
    static std::shared_ptr<Camera> camera;

    int selectShader;
    double lastTime = 0;
    double deltaTime = 0;
    std::queue<std::function<void()>> drawQueue = std::queue<std::function<void()>>();

    char* readShader(const char* filename);

public:
    Frustum frustum;
    void initShaderProgram(const char* vert, const char* frag);
    void drawObjectInstaced(const Object3D& obj, const ShaderOptions& options);
    static std::unique_ptr<Shader> create(const char* vert, const char* frag);

    Shader(const char* vert, const char* frag);
    void draw();
    
    void updateDeltaTime();
    glm::mat4 getModel();
    void set(const char* name, glm::vec3& data);
    void set(const char* name, int& data);
    void set(char* name, glm::mat4& data);
    void set(char* name, glm::mat3& data);
    
    // void addDraw(const Object3D& object);
    // void addDraw(const Object3D& object, const std::vector<glm::vec3>& instances);
    static void init(GLFWwindow *window, std::shared_ptr<Camera> camera);

    ~Shader();
};

