#include "engine/object.h"
#include <iostream>

const double gravity = 1.0;

class Gravity {
    static inline std::map<int, double> speedY;

    static constexpr double G_CONSTANT = 9.81; // Сила тяжести
    static constexpr double GROUND_LEVEL = 0.0; // Уровень земли

public:
    static void update(Object* object, double deltaTime, double otherLevel = 0.0) {
        int id = object->getId();
        glm::vec3 pos = object->getPosition();

        // 1. Если объект на земле, обнуляем скорость и не считаем гравитацию

        if (pos.y <= otherLevel) {
            pos.y = otherLevel;
            speedY[id] = 0;
            object->setPosition(pos);
            return; 
        }
            

        // 2. Считаем ускорение (просто G, без сложных дробей, если это не космос)
        double acceleration = G_CONSTANT;

        // 3. Обновляем скорость (падение вниз, поэтому минус)
        speedY[id] -= acceleration * deltaTime;

        // 4. Ограничиваем скорость (Terminal Velocity), чтобы объект не пролетал сквозь текстуры
        if (speedY[id] < -50.0) speedY[id] = -50.0;

        // 5. Обновляем позицию
        pos.y += speedY[id] * deltaTime;

        // Проверка: не провалились ли мы под землю в этом кадре?
        if (pos.y < GROUND_LEVEL) {
            pos.y = GROUND_LEVEL;
            speedY[id] = 0;
        }

        object->setPosition(pos);
    }
};