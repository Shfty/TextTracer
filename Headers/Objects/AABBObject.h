#ifndef AABBOBJECT_H
#define AABBOBJECT_H

#include "WorldObject.h"
#include "AABBGeometry.h"

class AABBObject : public WorldObject
{
public:
    AABBObject(const glm::vec3& position, const glm::mat4& rotation, const AABB& bounds);

    AABB GetBounds() { return dynamic_cast<AABBGeometry*>(m_geometry)->GetBounds(); }
    void SetBounds(const AABB& bounds) { dynamic_cast<AABBGeometry*>(m_geometry)->SetBounds(bounds); }
};

#endif // AABBOBJECT_H
