#pragma once

#include <glm/vec4.hpp>
#include <glm/ext/matrix_float4x4.hpp>

enum FrustumPlane {
    PLANE_LEFT,    // левая
    PLANE_RIGHT,   // правая
    PLANE_BOTTOM,  // нижняя
    PLANE_TOP,     // верхняя
    PLANE_NEAR,    // ближняя
    PLANE_FAR      // дальняя
};

class Frustum {
private:
    glm::vec4 planes[6];  // 6 плоскостей в формате (A, B, C, D)

    void normalizePlane(glm::vec4& plane);
    
public:
    // Обновляем фрустум на основе матрицы вида * проекции
    void update(const glm::mat4& viewProjectionMatrix);
    
    // Проверка точки
    bool isPointVisible(const glm::vec3& point) const;
    
    // Проверка сферы
    bool isSphereVisible(const glm::vec3& center, float radius) const;
    
    // Проверка AABB (для чанков)
    bool isAABBVisible(const glm::vec3& min, const glm::vec3& max) const;
    
    // Проверка AABB с углами (более точная)
    bool isAABBVisibleExact(const glm::vec3& min, const glm::vec3& max) const;
};