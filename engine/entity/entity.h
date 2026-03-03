#ifndef ENTITY_H
#define ENTITY_H

#include <glm/vector_relational.hpp>

#include "collision.cpp"

enum EntityType {
    BLOCK = 0,
};

class Entity {

public:
    static inline int count = 0;
    glm::vec3 rotate{0,0,0};
    EntityType type;
    glm::vec3 position;
    Object3D object;
    int id = 0;
    Entity(const Object3D& object, glm::vec3 position, EntityType type);
    bool constains(const glm::vec3& point) const;
    void setRotate(glm::vec3 rotate);
    glm::vec3 getRotate();
    Object3D getObject();
    EntityType getType();
    glm::vec3 getPosition();
    void setPosition(glm::vec3 position);
};

class Block: public Entity, public CollisionBox {
public:
    Block(const Object3D& object, glm::vec3 position);
    using 
        Entity::getPosition, 
        Entity::setPosition,
        Entity::getObject;
};


#endif

