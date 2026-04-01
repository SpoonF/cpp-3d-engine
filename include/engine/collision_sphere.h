#pragma once

#include "collision.h"

class CollisionSphere : public Collision {
protected:
    float radius = 1.15f;
public:
    CollisionSphere(const Object3D& object, glm::vec3 position, float radius);
    CollisionSphere(glm::vec3 position, float radius);

    bool checkCollision(CollisionSphere* other, CollisionInfo& info);
};