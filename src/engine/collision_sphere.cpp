#include "engine/collision_sphere.h"
#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>

#include "engine/object.h"
#include "utils/model.h"

CollisionSphere::CollisionSphere(glm::vec3 position, float radius): Collision(position) {
    this->radius = radius;
}
CollisionSphere::CollisionSphere(const Object3D& object, glm::vec3 position, float radius): Collision(object, position) {
    this->radius = radius;
}
bool CollisionSphere::checkCollision(CollisionSphere* other, CollisionInfo& info) {
    auto getDirection = [](glm::vec3 delta){
        glm::vec3 direction{0, 0, 0};
        size_t bigIndex = 0;
        for (size_t i = 1; i < 3; i++)
        {
            if(fabs(delta[bigIndex]) < fabs(delta[i])) {
                bigIndex = i;
            }
        }
        direction[bigIndex] = delta[bigIndex];

        return direction;
    };

    glm::vec3 delta = other->getPosition() - this->getPosition();

    if(radius == 0.f) {
        radius = this->radius;
    }
    
    float distance = glm::length(delta);
    float radiusSum = radius + other->radius;
    
    if (distance < radiusSum) {
        // Есть коллизия
        info.Normal = glm::normalize(delta);
        info.PenetrationDepth = radiusSum - distance;
        info.direction = getDirection(delta);
        info.collisionPoints.push_back(
            this->getPosition() + info.Normal
        );

        // printf("[debag]: direction data x: %f, y: %f, z: %f \n", info.direction.x, info.direction.y, info.direction.z);
        return true;
    }
    return false;
}