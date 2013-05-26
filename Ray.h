#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

#include "WorldObject.h"

struct Ray
{
    WorldObject* ParentObject;

    glm::vec4 Colour;

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

    Ray(const glm::vec3& origin, const glm::vec3& direction)
        :Origin(origin),
        Direction(direction)
    {
        NearPlane = 0.0f;
        FarPlane = 100.0f;
    }

    Ray(const glm::vec3& origin, const glm::vec3& direction, const float nearPlane, const float farPlane)
        :Origin(origin),
        Direction(direction),
        NearPlane(nearPlane),
        FarPlane(farPlane) {}
};

#endif // RAY_H
