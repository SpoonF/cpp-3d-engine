#pragma once

#include "collision.h"

class CollisionBox : public Collision {
protected:
    glm::vec3 size;
public:
    CollisionBox(const glm::vec3& position, const glm::vec3& size);
    bool check(CollisionBox* other, CollisionInfo& info);
    bool check(const CollisionBox& other, CollisionInfo& info);
    bool check(const glm::vec3& point);

    void setSize(glm::vec3 size);
    glm::vec3 getSize();
};