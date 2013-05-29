#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <sstream>

#include "DebugBox.h"
#include "WorldObject.h"
#include "SphereGeometry.h"

class SphereObject : public WorldObject
{
public:
    SphereObject(const glm::vec3& position, const glm::mat4& rotation, const float radius, const bool backface);

    float GetRadius() { return dynamic_cast<SphereGeometry*>(m_geometry)->GetRadius(); }
    float GetBackface() { return dynamic_cast<SphereGeometry*>(m_geometry)->GetBackface(); }

    void SetRadius(const float radius) { dynamic_cast<SphereGeometry*>(m_geometry)->SetRadius(radius); }
    void SetBackface(const bool backface) { dynamic_cast<SphereGeometry*>(m_geometry)->SetBackface(backface); }
};

#endif // SPHEREOBJECT_H
