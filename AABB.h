#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class Ray;
class IsectData;
class Camera;

struct AABB
{
    glm::vec3 Min = glm::vec3();
    glm::vec3 Max = glm::vec3();

    AABB() {}

    AABB(glm::vec3 minPoint, glm::vec3 maxPoint)
    {
        Min = minPoint;
        Max = maxPoint;
    }

    bool Contains(const glm::vec3& point);
    bool Intersects(const Ray& ray, IsectData* isectData, const Camera* camera);
};

#endif // AABB_H
