#ifndef ConvexPolyObject_H
#define ConvexPolyObject_H

#include <vector>
#include "WorldObject.h"

class ConvexPolyObject : public WorldObject
{
public:
    ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const int sides, const bool twoSided);
    ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const std::vector<glm::vec3>& vertices, const bool twoSided);

    bool IntersectsPortal(const Ray& ray, IsectData* isectData, const glm::mat4& cameraRotation);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);

    float Scale;
    bool TwoSided;

private:
    void calculateAABB();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData);
    void genObjectVertices(const int sides);
    void genWorldVertices();
    void genWorldEdges();

    void calculateWorldNormalDotPosition();

    std::vector<glm::vec3> m_objectVertices;
    std::vector<glm::vec3> m_worldVertices;
    std::vector<glm::vec3> m_worldEdges;

    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // ConvexPolyObject_H
