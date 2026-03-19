#ifndef ENTITY_H
#define ENTITY_H

#include <glm/vector_relational.hpp>

#include "collision.cpp"

enum EntityType {
    BLOCK,
    LIGHT,
    PLAYER,
};

class EntityOptions {
    protected:
        bool isLight = false;
    public:
        bool isLighting();
    
};

class Entity: virtual protected EntityOptions {

public:
    static inline int count = 0;
    glm::vec3 rotate{0,0,0};
    EntityType type;
    glm::vec3 position;
    Object3D object;
    int id = 0;
    bool isSelected = false;
    Entity(const Object3D& object, const glm::vec3& position, EntityType type);
    bool constains(const glm::vec3& point) const;
    Object3D getObject();
    EntityType getType();
    virtual glm::vec3 getPosition();
    virtual void setPosition(const glm::vec3& position);
};



class Block: virtual public Entity, virtual public CollisionBox {
public:
    Block(const Object3D& object, const glm::vec3& position);
    using
        Entity::getPosition,
        Entity::getObject;
    void setPosition(const glm::vec3& position) override ;
};

class Player: virtual public Entity, virtual public CollisionBox {
public:
    Player(const Object3D& object, const glm::vec3& position);
    using
        Entity::getPosition,
        Entity::getObject;
    void setPosition(const glm::vec3& position) override ;
};

class LightBlock: public Block {
protected:
    bool isLight = true;
public:
    LightBlock(const Object3D& object, const glm::vec3& position);
};


#endif

