#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "WorldObject.h"
#include "PlaneGeometry.h"

class PlaneObject : public WorldObject
{
public:
    PlaneObject(const glm::vec3& position, const glm::mat4& rotation, bool twoSided);

    bool GetTwoSided() { return dynamic_cast<PlaneGeometry*>(m_geometry)->GetTwoSided(); }
    void SetTwoSided(const bool twoSided) { dynamic_cast<PlaneGeometry*>(m_geometry)->SetTwoSided(twoSided); }
};

#endif // PLANEOBJECT_H
