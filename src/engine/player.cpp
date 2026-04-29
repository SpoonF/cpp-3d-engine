#include <memory>

#include "engine/camera.h"
#include "engine/object.h"
#include "engine/collision_box.h"
#include "engine/object/block.h"

class Player: public Object, public CollisionBox {
private:
    std::shared_ptr<Camera> camera = nullptr;
    Block* selectObject = nullptr;
public:
    Player(const glm::vec3& position) : Positionable(position), Object(position, ObjectType::PLAYER), CollisionBox(position, glm::vec3(2.f)) {}

    void setCamera(std::shared_ptr<Camera> camera) {
        this->camera = camera;
        this->updateCameraPosition();
    };
    void updateCameraPosition() {
        if(this->camera != nullptr) {
            this->camera->setPosition(this->position + glm::vec3(0,2,0));
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

    void setSelectedObject(Block* selectObject) {
        this->removeSelectedObject();
        this->selectObject = selectObject;
    };
    Block* getSelectedObject() {
        return this->selectObject;
    };
    void removeSelectedObject() {
        if(this->selectObject != nullptr) {
            this->selectObject != nullptr;
        }
    };
};