#ifndef PLANEGEOMETRY_H
#define PLANEGEOMETRY_H

#include "Geometry.h"

class PlaneGeometry : public Geometry
{
public:
    PlaneGeometry() {};
    ~PlaneGeometry() {};

    bool GetTwoSided() const { return m_twoSided; }

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);
    void SetTwoSided(const bool twoSided);

private:
    void calculateAABB();
    void calculateWorldNormalDotPosition();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera);
    bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera);

    bool m_twoSided;
    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // PLANEGEOMETRY_H
