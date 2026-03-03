#ifndef COLLISION_H
#define COLLISION_H

#include <glm/vec3.hpp>
#include <vector>

#include "../object.cpp"

class CollisionInfo {
public:
    glm::vec3 Normal;
    float PenetrationDepth;
    std::vector<glm::vec3> collisionPoints;
    glm::vec3 direction;
};

class Collision {
protected:
    glm::vec3 position;
    Object3D object;
public:
    Collision(const Object3D& object, glm::vec3 position);
    Collision(glm::vec3 position);

    virtual int getVertexCount();
    virtual glm::vec3 getVertex(int i);
    virtual void setPosition(const glm::vec3& pos);
    virtual glm::vec3 getPosition();
    // virtual bool checkCollision(Collision* other, CollisionInfo& info);

};

class CollisionBox : public Collision {
protected:
    glm::vec3 size;
public:
    CollisionBox(const Object3D& object, glm::vec3 position, glm::vec3 size);
    CollisionBox(glm::vec3 position, glm::vec3 size);
    void setSize(glm::vec3 size);
    glm::vec3 getSize();

    bool checkCollision(CollisionBox* other, CollisionInfo& info) ;
};

class CollisionSphere : public Collision {
protected:
    float radius = 1.15f;
public:
    CollisionSphere(const Object3D& object, glm::vec3 position, float radius);
    CollisionSphere(glm::vec3 position, float radius);

    bool checkCollision(CollisionSphere* other, CollisionInfo& info);
};

#endif

