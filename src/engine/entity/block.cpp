#include "engine/entity/block.h"
#include "memory"

std::shared_ptr<Object3D> getDefaultBlockObject(EntityType type) {
    static std::shared_ptr<Object3D> block = std::make_shared<Object3D>("./assets/models/cube.obj", "./assets/textures/ground.bmp");
    static std::shared_ptr<Object3D> light = std::make_shared<Object3D>("./assets/models/cube.obj");
    static std::shared_ptr<Object3D> slab = std::make_shared<Object3D>("./assets/models/slab.obj", "./assets/textures/ground.bmp");

    switch (type) {
        case EntityType::BLOCK:
            return block;
        case EntityType::SLAB:
            return slab;
        case EntityType::LIGHT:
            return light;
        default:
            return nullptr;
    }

    
}

Block::Block(const glm::vec3& position) : 
    Entity(getDefaultBlockObject(EntityType::BLOCK), position, EntityType::BLOCK), 
    CollisionBox(getDefaultBlockObject(EntityType::BLOCK), position, glm::vec3(2.f)) { }


void Block::setPosition(const glm::vec3& position) {
    Entity::setPosition(position);
    CollisionBox::setPosition(position);
}


Slab::Slab(const glm::vec3& position) : 
    Block(position), 
    Entity(getDefaultBlockObject(EntityType::SLAB), position, EntityType::SLAB), 
    CollisionBox(getDefaultBlockObject(EntityType::SLAB), position, glm::vec3(2.f, 1.f, 2.f)) {}

LightBlock::LightBlock(const glm::vec3& position) : 
    Block(position), 
    Entity(getDefaultBlockObject(EntityType::LIGHT), position, EntityType::LIGHT), 
    CollisionBox(getDefaultBlockObject(EntityType::LIGHT), position, glm::vec3(2.f)) {}