#ifndef AABBGEOMETRY_H
#define AABBGEOMETRY_H

#include "Geometry.h"

class AABBGeometry : public Geometry
{
public:
    AABBGeometry() {};
    ~AABBGeometry() {};

    AABB GetBounds() const { return m_bounds; }

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);
    void SetBounds(const AABB& bounds);

private:
    void calculateAABB();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera);
    bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera);
    glm::vec3 cardinalDirection(const glm::vec3& inVector);
};

#endif // AABBGEOMETRY_H
