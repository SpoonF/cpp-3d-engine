#pragma once

#include <glm/vector_relational.hpp>
#include <glm/vec3.hpp>
#include <glm/gtx/hash.hpp>
#include <unordered_set>
#include <memory>
#include <array>

class Object3D;

enum EntityType {
    BLOCK,
    LIGHT,
    PLAYER,
    SLAB,
};

class EntityOptions {
    protected:
        bool _isLight = false;
        bool _isCollise = true;
        glm::vec3 position;
    public:
        bool isLighting();
        bool isCollisable();
        void setCollisable(bool value);
};

class Entity: public EntityOptions {

public:
    static inline int count = 0;
    glm::vec3 rotate{0,0,0};
    EntityType type;
    glm::vec3 position;
    std::shared_ptr<Object3D> object;
    int id = 0;
    bool isSelected = false;
    Entity(std::shared_ptr<Object3D> object, const glm::vec3& position, EntityType type);
    Entity(const glm::vec3& position, EntityType type);
    bool contains(const glm::vec3& point) const;
    std::shared_ptr<Object3D> getObject();

    EntityType getType();
    bool isType(EntityType type);

    virtual glm::vec3 getPosition();
    virtual void setPosition(const glm::vec3& position);
    void move(const glm::vec3& position);

    bool isWithinDistance(const Entity& other, float distance) const;
    bool isWithinDistance(Entity* other, float distance) const;
    bool isWithinDistance(const glm::vec3 &position, float distance) const;

};