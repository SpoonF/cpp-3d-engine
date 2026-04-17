#pragma once

#include <vector>
#include <memory>
#include <map>

#include "utils/imageBMP.h"
#include "utils/model.h"

#include "shader.h"


class ShaderOptions;

enum ObjectType {
    OBJECT,
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
    static ObjectType type;
public:

    Object() = default;
    Object(const glm::vec3& position, ObjectType type);
    bool operator<(const Object& other) const;
    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    int getId();

    // static void render(const ShaderOptions& options);

    glm::vec3 getPosition() const;
    void setPosition(const glm::vec3& position);
    void move(const glm::vec3& position);
    ObjectType getType() const;

    ~Object() {
        // printf("Object %i destoyed", this->id);
    }

    static void init(const char* modelpath, const char* texturepath, const char* vert, const char* frag) {
        Model::getInstance(modelpath, type);
        imageBMP::getInstance(texturepath, type);
        Shader::getInstance(vert, frag, type);
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