#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class Ray;

struct AABB
{
    glm::vec3 Min;
    glm::vec3 Max;

    AABB(glm::vec3 minPoint, glm::vec3 maxPoint)
    {
        Min = minPoint;
        Max = maxPoint;
    }

    bool Intersects(Ray& ray);
};

#endif // AABB_H
