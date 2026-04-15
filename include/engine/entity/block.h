#pragma once

#include "../entity.h"
#include "../collision_box.h"

extern std::shared_ptr<Object3D> getDefaultBlockObject(EntityType type);

class Block: virtual public Entity, virtual public CollisionBox {
public:
    Block(const glm::vec3& position);
    using
        Entity::getPosition,
        Entity::getObject;
    void setPosition(const glm::vec3& position) override ;
};

class Slab: virtual public Block {
public:
    Slab(const glm::vec3& position);
};

class LightBlock: virtual public Block {
protected:
    bool isLight = true;
public:
    LightBlock(const glm::vec3& position);
};