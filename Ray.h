#ifndef RAY_H
#define RAY_H

#include <glm/glm.hpp>

#include "IsectData.h"

struct distanceSortPredicate
{
    bool operator()( const IsectData* lx, const IsectData* rx ) const {
    	return lx->Distance < rx->Distance;
    }
};

struct Ray
{
    WorldObject* ParentObject;

    glm::vec3 Origin;
    glm::vec3 Direction;

    float NearPlane;
    float FarPlane;

    Ray(const glm::vec3& origin,
        const glm::vec3& direction,
        const float nearPlane,
        const float farPlane,
        const WorldObject* parentObject)
        :
        ParentObject((WorldObject*)parentObject),
        Origin(origin),
        Direction(direction),
        NearPlane(nearPlane),
        FarPlane(farPlane) {}
};

#endif // RAY_H
