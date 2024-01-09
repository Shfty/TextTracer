#ifndef CONVEXPOLYGEOMETRY_H
#define CONVEXPOLYGEOMETRY_H

#include <vector>

#include "Geometry.h"

class ConvexPolyGeometry : public Geometry
{
public:
    ConvexPolyGeometry() {};
    ~ConvexPolyGeometry() {};

    bool GetTwoSided() const { return m_twoSided; }
    float GetScale() const { return m_scale; }
    int GetSides() const { return m_sides; }
    std::vector<glm::vec3> GetObjectVertices() const { return m_objectVertices; }


    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);
    void SetTwoSided(const bool twoSided);
    void SetScale(const float scale);
    void SetSides(const int sides);
    void SetObjectVertices(const std::vector<glm::vec3>& vertices);

private:
    void genObjectVertices();
    void genWorldVertices();
    void genWorldEdges();
    void calculateWorldNormalDotPosition();

    void calculateAABB();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera);
    bool intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera);

    std::vector<glm::vec3> m_objectVertices;
    std::vector<glm::vec3> m_worldVertices;
    std::vector<glm::vec3> m_worldEdges;

    bool m_twoSided;
    float m_scale;
    int m_sides;

    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // CONVEXPOLYGEOMETRY_H
