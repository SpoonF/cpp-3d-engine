#pragma once

#include "../entity.h"
#include "../collision_box.h"

class Block: virtual public Entity, virtual public CollisionBox {
public:
    Block(const Object3D& object, const glm::vec3& position);
    Block(const glm::vec3& position);
    using
        Entity::getPosition,
        Entity::getObject;
    void setPosition(const glm::vec3& position) override ;
};

class Slab: virtual public Block {
public:
    Slab(const Object3D& object, const glm::vec3& position);
    Slab(const glm::vec3& position);
};

class LightBlock: virtual public Block {
protected:
    bool isLight = true;
public:
    LightBlock(const Object3D& object, const glm::vec3& position);
    LightBlock(const glm::vec3& position);
};