#pragma once

#include <glm/vector_relational.hpp>
#include <glm/vec3.hpp>
#include <memory>

class Object3D;

enum EntityType {
    BLOCK,
    LIGHT,
    PLAYER,
    SLAB,
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
    Entity(const glm::vec3& position, EntityType type);
    bool constains(const glm::vec3& point) const;
    Object3D getObject();
    EntityType getType();
    virtual glm::vec3 getPosition();
    virtual void setPosition(const glm::vec3& position);
};