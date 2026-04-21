#include "engine/object/block.h"
#include "memory"
#include <iostream>

Block::Block(const glm::vec3& position) : 
    Positionable(position), Object(position, ObjectType::BLOCK), CollisionBox(position, glm::vec3(2.f)) { }

