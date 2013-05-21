#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

#include "WorldObject.h"

struct Ray
{
    glm::vec3 Origin;
    glm::vec3 Direction;
    float NearPlane;
    float FarPlane;

    Ray()
    {
        Origin = glm::vec3(0, 0, 0);
        Direction = glm::vec3(0, 0, 0);
        NearPlane = 0.0f;
        FarPlane = 100.0f;
    }

    Ray(glm::vec3 origin, glm::vec3 direction, float nearPlane, float farPlane)
        :Origin(origin),
        Direction(direction),
        NearPlane(nearPlane),
        FarPlane(farPlane) {}
};

#endif // RAY_H
