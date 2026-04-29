#pragma once

#include <glm/gtc/quaternion.hpp>

#include "collision.h"

class CollisionBox : public Collision {
protected:
    glm::vec3 center;
    glm::vec3 size;
    glm::quat quaternion;
public:
    CollisionBox(const glm::vec3& position, const glm::vec3& size);
    bool check(CollisionBox* other);
    bool check(CollisionBox* other, CollisionInfo& info);
    glm::vec3 getPenitrationDepth(CollisionBox* other);
    glm::vec3 resolveCollision(CollisionBox* other, const glm::vec3& overlap);

    bool check(const glm::vec3& point);
    bool checkCollise(CollisionBox* other, CollisionInfo& info);


    void setSize(glm::vec3 size);
    glm::vec3 getSize();
private:
    static glm::vec3 quanRotation(const glm::vec3& vec, const glm::quat& quat);
    static std::array<glm::vec3, 8> getPoint(CollisionBox *box);
    static std::vector<glm::vec3> getAxis(const std::array<glm::vec3, 8>& a, const std::array<glm::vec3, 8>& b);
    static float projVec3(const glm::vec3& v, glm::vec3 a);
    static glm::vec3 intersectionOfProj(const std::array<glm::vec3, 8>& a, const std::array<glm::vec3, 8>& b, const std::vector<glm::vec3>& axis);
    static void projectAxis(float& min, float& max, const std::array<glm::vec3, 8>& points, const glm::vec3& axis);

};