#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <vector>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Geometry.h"

class WorldObject
{
public:
    WorldObject* Parent = NULL;

    bool CastShadow = true;
    bool Fullbright = false;

    WorldObject() {}
    virtual ~WorldObject() { delete m_geometry; };

    glm::vec3 GetPosition() const { return m_geometry->GetPosition(); }
    glm::mat4 GetRotation() const { return m_geometry->GetRotation(); }
    glm::vec4 GetColour() const { return m_geometry->GetColour(); }
    Geometry* GetGeometry() const { return m_geometry; }
    WorldObject* GetExitPortal() const { return m_exitPortal; }

    void SetPosition(const glm::vec3& position) { m_geometry->SetPosition(position); }
    void SetRotation(const glm::mat4& rotation) { m_geometry->SetRotation(rotation); }
    void SetColour(const glm::vec4& colour) { m_geometry->SetColour(colour); }
    void SetExitPortal(const WorldObject* exitPortal) { m_exitPortal = (WorldObject*)exitPortal; }

    bool Intersects(const Ray& ray, IsectData* isectData, const Camera* camera, const bool portal);

protected:
    Geometry* m_geometry;

    WorldObject* m_exitPortal = NULL;
};

#endif // WORLDOBJECT_H
