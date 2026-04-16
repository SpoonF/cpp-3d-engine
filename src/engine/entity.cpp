// #include "engine/entity.h"
// #include "engine/entity/block.h"
// #include "engine/entity/player.h"

// #include <memory>

// #include "engine/object.h"

// bool EntityOptions::isLighting() {
//     return this->_isLight;
// }

// bool EntityOptions::isCollisable()
// {
//     return this->_isCollise;
// }

// void EntityOptions::setCollisable(bool value)
// {
//     this->_isCollise = value;
// }

// Entity::Entity(std::shared_ptr<Object> object, const glm::vec3 &position, EntityType type)
// {
//     this->position = position;
//     this->object = object;
//     this->type = type;
//     this->count++;
//     this->id = count;
// }
// Entity::Entity(const glm::vec3& position, EntityType type) {
//     this->position = position;
//     this->object = {};
//     this->type = type;
//     this->count++;
//     this->id = count;
// }
// bool Entity::contains(const glm::vec3& point) const {
//     return glm::all(glm::greaterThanEqual(point, position)) && 
//     glm::all(glm::lessThanEqual(point, position + 1.f));
// }
// std::shared_ptr<Object> Entity::getObject() {
//     return this->object;
// }
// EntityType Entity::getType() {
//     return this->type;
// }
// bool Entity::isType(EntityType type)
// {
//     return this->type == type;
// }


// bool Entity::isWithinDistance(const Entity &other, float distance) const
// {
//     glm::vec3 dist = this->position - other.position;

//     if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
//         return false;
//     }
//     return true;
// }

// bool Entity::isWithinDistance(Entity* other, float distance) const
// {
//     glm::vec3 dist = this->position - other->position;

//     if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
//         return false;
//     }
//     return true;
// }

// bool Entity::isWithinDistance(const glm::vec3 &posision, float distance) const
// {
//     glm::vec3 dist = this->position - posision;

//     if(abs(dist.x) > distance || abs(dist.y) > distance || abs(dist.z) > distance) {
//         return false;
//     }
//     return true;
// }


// std::shared_ptr<Entity> Entity::create(EntityType type, glm::vec3 position) {
//     switch (type) {
//         case EntityType::BLOCK:
//             return std::make_shared<Block>(position);
//         case EntityType::SLAB:
//             return std::make_shared<Slab>(position);
//         case EntityType::LIGHT:
//             return std::make_shared<LightBlock>(position);
//         // case EntityType::PLAYER:
//         //     return std::make_shared<Player>(position);
//         default:
//             return nullptr;
//     }
// }