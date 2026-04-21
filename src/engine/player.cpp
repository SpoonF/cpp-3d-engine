#include <memory>

#include "engine/camera.h"
#include "engine/object.h"

class Player: public Object, public CollisionBox {
private:
    std::shared_ptr<Camera> camera = nullptr;
public:
    Player(const glm::vec3& position) : Positionable(position), Object(position, ObjectType::PLAYER), CollisionBox(position, glm::vec3(2.f)) {}

    void setCamera(std::shared_ptr<Camera> camera) {
        this->camera = camera;
        this->camera->setPosition(this->position);
    };
    void updateCameraPosition() {
        if(this->camera != nullptr) {
            this->camera->setPosition(this->position);
        }
    }

    void setPosition(const glm::vec3& position) override {
        this->position = position;
        this->updateCameraPosition();
    };
    void move(const glm::vec3& position) override {
        this->position += position;
        this->updateCameraPosition();
    };
};