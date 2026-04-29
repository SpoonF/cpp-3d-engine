#pragma once

#include <glm/vec3.hpp>
#include <vector>
#include <memory>

#include "engine/traits/positionable.h"

// class Positionable;


class CollisionInfo {
public:
    glm::vec3 Normal;
    glm::vec3 PenetrationDepth;
    std::vector<glm::vec3> collisionPoints;
    glm::vec3 direction;
};

class Collision: virtual public Positionable {
public:
    Collision(const glm::vec3& position);

    bool isWithinDistance(const glm::vec3& pos, float distance) const;
};


