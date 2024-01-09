#ifndef SPHEREGEOMETRY_H
#define SPHEREGEOMETRY_H

#include "Geometry.h"

class SphereGeometry : public Geometry
{
public:
    SphereGeometry() {};
    ~SphereGeometry() {};

    float GetRadius() { return m_radius; }
    float GetBackface() { return m_backface; }

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);
    void SetRadius(const float radius);
    void SetBackface(const bool backface);

private:
    void calculateAABB();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera);
    bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera);

    float m_radius = 1.0f;
    bool m_backface = false;
};

#endif // SPHEREGEOMETRY_H
