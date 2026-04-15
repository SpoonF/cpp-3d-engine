#pragma once

#include "collision.h"

class CollisionBox : public Collision {
protected:
    glm::vec3 size;
public:
    CollisionBox(std::shared_ptr<Object3D> object, glm::vec3 position, glm::vec3 size);
    CollisionBox(glm::vec3 position, glm::vec3 size);
    void setSize(glm::vec3 size);
    glm::vec3 getSize();

    bool checkCollision(CollisionBox* other, CollisionInfo& info) ;
};