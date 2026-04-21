#pragma once

#include <glm/vec3.hpp>

class Positionable {
protected:
    glm::vec3 position;
public:
    Positionable(const glm::vec3& position);
    virtual glm::vec3 getPosition() const;
    virtual void setPosition(const glm::vec3& position);
    virtual void move(const glm::vec3& position);
};