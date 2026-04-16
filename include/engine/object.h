#pragma once

#include <vector>
#include <memory>
#include <map>

#include "utils/imageBMP.h"
#include "utils/model.h"
#include <sys/types.h>
class Shader;

class ShaderOptions;

enum ObjectType {
    BLOCK,
    LIGHT,
    PLAYER,
    SLAB,
};

class Object {
protected:
    uint id;
    static inline uint count = 0;
    glm::vec3 position;
    ObjectType type;

    Model model;
    imageBMP texture;
public:
    std::shared_ptr<Shader> shader;

    Object() = default;
    Object(const Model& model, const imageBMP& texture, const glm::vec3& position, ObjectType type);
    Object(const Model& model, const glm::vec3& position, ObjectType type);
    Object(const char* modelpath, const char* texturepath, const glm::vec3& position, ObjectType type);
    Object(const char* modelpath, const glm::vec3& position, ObjectType type);
    bool operator<(const Object& other) const;
    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    void render(const ShaderOptions& options) const;

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& position);
    void setShader(std::shared_ptr<Shader> shader);

    ~Object() {
        printf("Object %i destoyed", this->id);
    }
};

// class Entity: public EntityOptions {

// public:
//     virtual glm::vec3 getPosition();
//     virtual void setPosition(const glm::vec3& position);
//     void move(const glm::vec3& position);

//     bool isWithinDistance(const Entity& other, float distance) const;
//     bool isWithinDistance(Entity* other, float distance) const;
//     bool isWithinDistance(const glm::vec3 &position, float distance) const;

//     static std::shared_ptr<Entity> create(EntityType type, glm::vec3 position);
// };