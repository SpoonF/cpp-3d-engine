#include "engine/entity.h"
#include "engine/object.h"

bool EntityOptions::isLighting() {
    return this->_isLight;
}

bool EntityOptions::isCollisable()
{
    return this->_isCollise;
}

void EntityOptions::setCollisable(bool value)
{
    this->_isCollise = value;
}

Entity::Entity(const Object3D &object, const glm::vec3 &position, EntityType type)
{
    this->position = position;
    this->object = object;
    this->type = type;
    this->count++;
    this->id = count;
}
Entity::Entity(const glm::vec3& position, EntityType type) {
    this->position = position;
    this->object = {};
    this->type = type;
    this->count++;
    this->id = count;
}
bool Entity::contains(const glm::vec3& point) const {
    return glm::all(glm::greaterThanEqual(point, position)) && 
    glm::all(glm::lessThanEqual(point, position + 1.f));
}
Object3D Entity::getObject() {
    return this->object;
}
EntityType Entity::getType() {
    return this->type;
}
bool Entity::isType(EntityType type)
{
    return this->type == type;
}
glm::vec3 Entity::getPosition() {
    return this->position;
};
void Entity::setPosition(const glm::vec3& position) {
    this->position = position;
};

void Entity::move(const glm::vec3 &position)
{
    this->position += position;
}

bool Entity::isWithinDistance(const Entity &other, float distance) const
{
    glm::vec3 dist = this->position - other.position;

    if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
        return false;
    }
    return true;
}

bool Entity::isWithinDistance(Entity* other, float distance) const
{
    glm::vec3 dist = this->position - other->position;

    if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
        return false;
    }
    return true;
}

bool Entity::isWithinDistance(const glm::vec3 &posision, float distance) const
{
    glm::vec3 dist = this->position - posision;

    if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
        return false;
    }
    return true;
}
