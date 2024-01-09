#ifndef DISCGEOMETRY_H
#define DISCGEOMETRY_H

#include "Geometry.h"

class DiscGeometry : public Geometry
{
public:
    DiscGeometry() {};
    ~DiscGeometry() {};

    bool GetTwoSided() const { return m_twoSided; }
    float GetScale() const { return m_scale; }

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);
    void SetTwoSided(const bool twoSided);
    void SetScale(const float scale);

private:
    void calculateAABB();
    void calculateWorldNormalDotPosition();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera);
    bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera);

    bool m_twoSided;
    float m_scale;
    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // DISCGEOMETRY_H
