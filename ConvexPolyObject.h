#ifndef ConvexPolyObject_H
#define ConvexPolyObject_H

#include <vector>
#include "WorldObject.h"
#include "ConvexPolyGeometry.h"

class ConvexPolyObject : public WorldObject
{
public:
    ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const int sides, const bool twoSided);
    ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const std::vector<glm::vec3>& vertices, const bool twoSided);

    float GetScale() const { return dynamic_cast<ConvexPolyGeometry*>(m_geometry)->GetScale(); }
    bool GetTwoSided() const { return dynamic_cast<ConvexPolyGeometry*>(m_geometry)->GetTwoSided(); }
    int GetSides() const { return dynamic_cast<ConvexPolyGeometry*>(m_geometry)->GetSides(); }
    std::vector<glm::vec3> GetObjectVertices() const { return dynamic_cast<ConvexPolyGeometry*>(m_geometry)->GetObjectVertices(); }

    void SetScale(const float scale) { dynamic_cast<ConvexPolyGeometry*>(m_geometry)->SetScale(scale); }
    void SetTwoSided(const bool twoSided) { dynamic_cast<ConvexPolyGeometry*>(m_geometry)->SetTwoSided(twoSided); }
    void SetSides(const int sides) { dynamic_cast<ConvexPolyGeometry*>(m_geometry)->SetSides(sides); }
    void SetObjectVertices(const std::vector<glm::vec3>& vertices) { dynamic_cast<ConvexPolyGeometry*>(m_geometry)->SetObjectVertices(vertices); }
};

#endif // ConvexPolyObject_H
