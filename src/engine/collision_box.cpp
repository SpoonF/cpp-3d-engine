#include "engine/collision_box.h"

#include <cmath>
#include <glm/ext/quaternion_geometric.hpp>

#include "engine/object.h"
#include <iostream>


CollisionBox::CollisionBox(const glm::vec3& position, const glm::vec3& size): Positionable(position), Collision(position) {
    this->center = position + this->size / 2.f; 
    this->size = size;
    this->quaternion = glm::quat();
};

void CollisionBox::setSize(glm::vec3 size) {
    this->size = size;
};
glm::vec3 CollisionBox::getSize() {
    return this->size;
};
bool CollisionBox::check(const glm::vec3 &point)
{
    bool collision = this->position.x <= point.x && this->position.x + this->size.x >= point.x &&
                    this->position.y <= point.y && this->position.y + this->size.y >= point.y &&
                    this->position.y <= point.y && this->position.y + this->size.y >= point.y;
    return collision;
}

bool CollisionBox::check(CollisionBox* other) {
    glm::vec3 pos = this->position;
    glm::vec3 size = this->size;
    glm::vec3 oPos = other->position;
    glm::vec3 oSize = other->size;

    glm::vec3 delta = oPos - pos;
    
    bool collision = 
        (oPos.x <= (pos.x + size.x) && (oPos.x + oSize.x) >= pos.x) &&
        (oPos.y <= (pos.y + size.y) && (oPos.y + oSize.y) >= pos.y) &&
        (oPos.z <= (pos.z + size.z) && (oPos.z + oSize.z) >= pos.z);
    
    return collision;
}

bool CollisionBox::check(CollisionBox* other, CollisionInfo& info) {
    
    bool collision = this->check(other);
    
    if (collision) {
        info.PenetrationDepth = this->getPenitrationDepth(other);
    }
    
    return collision;
}




glm::vec3 CollisionBox::getPenitrationDepth(CollisionBox* other) {
    glm::vec3 delta = this->position - other->position;
    glm::vec3 overlap;

    overlap.x = std::max(0.f, (this->size.x / 2.f + other->size.x / 2.f) - abs(delta.x));
    overlap.y = std::max(0.f, (this->size.y / 2.f + other->size.y / 2.f) - abs(delta.y));
    overlap.z = std::max(0.f, (this->size.z / 2.f + other->size.z / 2.f) - abs(delta.z));

    return overlap;
}

glm::vec3 CollisionBox::resolveCollision(CollisionBox* other, const glm::vec3& overlap) {
    glm::vec3 delta = this->position - other->position;
    
    if(overlap.x <= overlap.y && overlap.x <= overlap.z) {
        return glm::vec3((delta.x > 0 ? overlap.x : -overlap.x), 0, 0);
    } else if(overlap.y <= overlap.x && overlap.y <= overlap.z) {
        return glm::vec3(0, (delta.y > 0 ? overlap.y : -overlap.y), 0);
    } else {
        return glm::vec3(0, 0, (delta.z > 0 ? overlap.z : -overlap.z));
    }
}

glm::vec3 CollisionBox::quanRotation(const glm::vec3& vec, const glm::quat& quat) {
    float u0 = vec.x * quat.x + vec.y * quat.y + vec.z * quat.z;
    float u1 = vec.x * quat.w - vec.y * quat.z + vec.z * quat.y;
    float u2 = vec.x * quat.z + vec.y * quat.w - vec.z * quat.x;
    float u3 = -vec.x * quat.y + vec.y * quat.x + vec.z * quat.w;

    glm::quat m(u1,u2,u3,u0);

    glm::vec3 result;
    result.x = quat.w * m.x + quat.x * m.w + quat.y * m.z - quat.z * m.y;
    result.y = quat.w * m.y - quat.x * m.z + quat.y * m.w + quat.z * m.x;
    result.z = quat.w * m.z + quat.x * m.y - quat.y * m.x + quat.z * m.w;

    return result;
}

std::array<glm::vec3, 8> CollisionBox::getPoint(CollisionBox *box) {
    std::array<glm::vec3, 8> points;

    points[0] = box->center - box->size / 2.0f;
    points[1] = points[0] + glm::vec3(box->size.x, 0, 0);
    points[2] = points[0] + glm::vec3(0, box->size.y, 0);
    points[3] = points[0] + glm::vec3(0, 0, box->size.z);

    points[4] = box->center + box->size / 2.0f;
    points[5] = points[4] - glm::vec3(box->size.x, 0, 0);
    points[6] = points[4] - glm::vec3(0, box->size.y, 0);
    points[7] = points[4] - glm::vec3(0, 0, box->size.z);

    // for (int i = 0; i < 8; i++) {
    //     points[i] -= box->center;
    //     points[i] = quanRotation(points[i], box->quaternion);
    //     points[i] += box->center;
    // }

    return points;
}

std::vector<glm::vec3> CollisionBox::getAxis(const std::array<glm::vec3, 8>& a, const std::array<glm::vec3, 8>& b) {
    glm::vec3 A;    
    glm::vec3 B;
    
    std::vector<glm::vec3> axis;

    for (int i = 1; i < 4; i++) {
        A = a[i] - a[0];
        B = a[(i+1)%3+1] - a[0];
        axis.push_back(glm::normalize(glm::cross(A, B)));
    }

    

    for (int i = 1; i < 4; i++) {
        A = b[i] - b[0];
        B = b[(i+1)%3+1] - b[0];
        axis.push_back(glm::normalize(glm::cross(A, B)));
    }

    for (int i = 1; i < 4; i++) {
        A = b[i] - b[0];

        for (int j = 1; j < 4; j++) {
            B = b[j] - b[0];
            
            if(glm::length2(glm::cross(A, B)) != 0) {
                axis.push_back(glm::normalize(glm::cross(A, B)));
            }
        }
    }

    return axis;
}

float CollisionBox::projVec3(const glm::vec3& v, glm::vec3 a) {
    a = glm::normalize(a);
    auto result = glm::dot(v, a) / glm::length2(a);

    return result;
}

glm::vec3 CollisionBox::intersectionOfProj(const std::array<glm::vec3, 8>& a, const std::array<glm::vec3, 8>& b, const std::vector<glm::vec3>& axis) {
    glm::vec3 norm(10000);
    
    for (int j = 0; j < axis.size(); j++) {
        float maxA;
        float minA;
        projectAxis(minA, maxA, a, axis[j]);

        float maxB;
        float minB;
        projectAxis(minB, maxB, b, axis[j]);

        std::array<float, 4> points = {minA, maxA, minB, maxB};
        std::sort(points.begin(), points.end());

        float sum = (maxB - minB) + (maxA - minA);

        float len = abs(points[3] - points[0]);

        if(sum <= len) {
            return glm::vec3(0);
        }

        float dl = abs(points[2] - points[1]);


        if(dl < glm::length2(norm)) {
            norm = axis[j] * dl;
            if(points[0] != minA) {
                norm = -norm;
            } 

        }
    }
    return norm;
    
}

void CollisionBox::projectAxis(float& min, float& max, const std::array<glm::vec3, 8>& points, const glm::vec3& axis) {
    max = projVec3(points[0], axis);
    min = max;

    for (int i = 1; i < points.size(); i++)
    {
        float tmp = projVec3(points[i], axis);

        max = std::max(max, tmp);
        min = std::min(min, tmp);
    }
    
}

bool CollisionBox::checkCollise(CollisionBox* other, CollisionInfo& info) {
    auto points = getPoint(this);
    auto oPoints = getPoint(other);

    auto axis = getAxis(points, oPoints);

    auto result = intersectionOfProj(points, oPoints, axis);
    
    if(result != glm::vec3(0)) {
        info.Normal = result;

        return true;
    }
    return false;
}