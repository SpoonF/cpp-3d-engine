#pragma once

#include "../object.h"
#include "../collision_box.h"

class Block: public Object, public CollisionBox {
public:
    Block(const glm::vec3& position);
};