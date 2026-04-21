#pragma once

#include <vector>
#include <memory>
#include <map>

#include "utils/imageBMP.h"
#include "utils/model.h"
#include "traits/positionable.h"

#include "shader.h"




class ShaderOptions;

struct Options {
    bool isLighting; 
};

enum ObjectType {
    OBJECT,
    BLOCK,
    LIGHT,
    PLAYER,
    SLAB,
};


class Object: virtual public Positionable {
protected:
    uint id;
    static inline uint count = 0;
    ObjectType type;
    
public:

    Object() = default;
    Object(const glm::vec3& position, ObjectType type);
    bool operator<(const Object& other) const;
    bool operator==(const Object& other) const;
    bool operator!=(const Object& other) const;

    std::unique_ptr<Options> options;
    Options* getOptions();

    int getId();

    // static void render(const ShaderOptions& options);

    ~Object() {
        // printf("Object %i destoyed", this->id);
    }

    template <ObjectType type>
    static void init(const char* modelpath, const char* texturepath, const char* vert, const char* frag);
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