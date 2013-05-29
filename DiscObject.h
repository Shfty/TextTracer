#ifndef DISCOBJECT_H
#define DISCOBJECT_H

#include <vector>
#include "WorldObject.h"
#include "DiscGeometry.h"

class DiscObject : public WorldObject
{
public:
    DiscObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const bool twoSided);

    bool GetTwoSided() { return dynamic_cast<DiscGeometry*>(m_geometry)->GetTwoSided(); }
    float GetScale() { return dynamic_cast<DiscGeometry*>(m_geometry)->GetScale(); }

    void SetTwoSided(const bool twoSided) { dynamic_cast<DiscGeometry*>(m_geometry)->SetTwoSided(twoSided); }
    void SetScale(const float scale) { dynamic_cast<DiscGeometry*>(m_geometry)->SetScale(scale); }
};

#endif // DISCOBJECT_H
