#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <glm/glm.hpp>
#include "AABB.h"

class IsectData;
class Camera;
class Ray;

class Geometry
{
public:
    Geometry() {}
    virtual ~Geometry() {}

    glm::vec3 GetPosition() { return m_position; }
    glm::mat4 GetRotation() { return m_rotation; }
    glm::vec4 GetColour() { return m_colour; }
    bool GetUseBoundingBox() { return m_useBBox; }

    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual void SetRotation(const glm::mat4& rotation) = 0;
    void SetColour(const glm::vec4& colour) { m_colour = colour; }
    void SetUseBoundingBox(const bool useBBox) { m_useBBox = useBBox; }
    void SetCSGSubtract(const Geometry* csgSubtract) { m_csgSubtract = (Geometry*)csgSubtract; }

    bool Intersects(const Ray& ray, IsectData* isectData, const Camera* camera, const bool portal);

    bool UseBoundingBox = false;

protected:
    virtual void calculateAABB() = 0;
    bool intersectsAABB(const Ray& ray);
    virtual bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera) = 0;
    virtual bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera) = 0;

    AABB m_bounds = AABB(glm::vec3(), glm::vec3(), glm::vec3());
    glm::vec3 m_position;
    glm::mat4 m_rotation;
    glm::vec4 m_colour;
    bool m_useBBox;

    Geometry* m_csgSubtract = NULL;
};

#endif // GEOMETRY_H
