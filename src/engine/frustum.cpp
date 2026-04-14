#include "engine/frustum.h"

#include <glm/gtc/matrix_access.hpp>

void Frustum::update(const glm::mat4& viewProj) {
    // Матрица viewProj = projection * view
    // OpenGL использует column-major порядок
    
    // Извлекаем строки матрицы (в column-major, строки это столбцы в памяти)
    glm::vec4 row0 = glm::row(viewProj, 0);
    glm::vec4 row1 = glm::row(viewProj, 1);
    glm::vec4 row2 = glm::row(viewProj, 2);
    glm::vec4 row3 = glm::row(viewProj, 3);
    
    // Левая плоскость (Plane: x = -w в clip space)
    // Уравнение: row3 + row0
    planes[PLANE_LEFT] = row3 + row0;
    normalizePlane(planes[PLANE_LEFT]);
    
    // Правая плоскость (Plane: x = w в clip space)
    // Уравнение: row3 - row0
    planes[PLANE_RIGHT] = row3 - row0;
    normalizePlane(planes[PLANE_RIGHT]);
    
    // Нижняя плоскость (Plane: y = -w в clip space)
    // Уравнение: row3 + row1
    planes[PLANE_BOTTOM] = row3 + row1;
    normalizePlane(planes[PLANE_BOTTOM]);
    
    // Верхняя плоскость (Plane: y = w в clip space)
    // Уравнение: row3 - row1
    planes[PLANE_TOP] = row3 - row1;
    normalizePlane(planes[PLANE_TOP]);
    
    // Ближняя плоскость (Plane: z = -w в clip space)
    // Уравнение: row3 + row2
    planes[PLANE_NEAR] = row3 + row2;
    normalizePlane(planes[PLANE_NEAR]);
    
    // Дальняя плоскость (Plane: z = w в clip space)
    // Уравнение: row3 - row2
    planes[PLANE_FAR] = row3 - row2;
    normalizePlane(planes[PLANE_FAR]);
}

// Нормализация плоскости (делаем длину нормали = 1)
void Frustum::normalizePlane(glm::vec4& plane) {
    float length = glm::length(glm::vec3(plane));
    if (length > 0.0001f) {
        plane /= length;
    }
}

bool Frustum::isPointVisible(const glm::vec3& point) const {
    for (int i = 0; i < 6; i++) {
        // Расстояние от точки до плоскости
        float distance = planes[i].x * point.x + 
                         planes[i].y * point.y + 
                         planes[i].z * point.z + 
                         planes[i].w;
        
        // Если точка с отрицательной стороны хотя бы одной плоскости — не видна
        if (distance < 0) {
            return false;
        }
    }
    return true;
}

bool Frustum::isSphereVisible(const glm::vec3& center, float radius) const {
    for (int i = 0; i < 6; i++) {
        // Расстояние от центра сферы до плоскости
        float distance = planes[i].x * center.x + 
                         planes[i].y * center.y + 
                         planes[i].z * center.z + 
                         planes[i].w;
        
        // Если сфера полностью за плоскостью
        if (distance < -radius) {
            return false;
        }
        
        // Если сфера пересекает плоскость — считаем видимой
        // (дальше проверят другие плоскости)
    }
    return true;
}

bool Frustum::isAABBVisible(const glm::vec3& min, const glm::vec3& max) const {
    // Для каждой плоскости проверяем самую дальнюю точку AABB
    for (int i = 0; i < 6; i++) {
        const glm::vec4& plane = planes[i];
        
        // Находим угол AABB, который ближе всего к плоскости
        // (максимальное отрицательное расстояние)
        glm::vec3 p = min;
        
        // Выбираем правильный угол для этой плоскости
        if (plane.x > 0) p.x = max.x;
        if (plane.y > 0) p.y = max.y;
        if (plane.z > 0) p.z = max.z;
        
        // Расстояние от этого угла до плоскости
        float distance = plane.x * p.x + plane.y * p.y + plane.z * p.z + plane.w;
        
        // Если даже ближайший угол снаружи (отрицательная сторона) — AABB не виден
        if (distance < 0) {
            return false;
        }
    }
    
    return true;
}

bool Frustum::isAABBVisibleExact(const glm::vec3& min, const glm::vec3& max) const {
    // 8 углов AABB
    glm::vec3 corners[8] = {
        glm::vec3(min.x, min.y, min.z),
        glm::vec3(max.x, min.y, min.z),
        glm::vec3(min.x, max.y, min.z),
        glm::vec3(max.x, max.y, min.z),
        glm::vec3(min.x, min.y, max.z),
        glm::vec3(max.x, min.y, max.z),
        glm::vec3(min.x, max.y, max.z),
        glm::vec3(max.x, max.y, max.z)
    };
    
    for (int i = 0; i < 6; i++) {
        const glm::vec4& plane = planes[i];
        
        // Проверяем все углы
        bool hasPositive = false;
        for (int j = 0; j < 8; j++) {
            float distance = plane.x * corners[j].x + 
                             plane.y * corners[j].y + 
                             plane.z * corners[j].z + 
                             plane.w;
            
            if (distance >= 0) {
                hasPositive = true;
                break;
            }
        }
        
        // Если все углы с отрицательной стороны — AABB не виден
        if (!hasPositive) {
            return false;
        }
    }
    
    return true;
}