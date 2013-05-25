#ifndef ConvexPolyObject_H
#define ConvexPolyObject_H

#include <vector>
#include "WorldObject.h"

class ConvexPolyObject : public WorldObject
{
public:
    ConvexPolyObject(glm::vec3 position, glm::mat4 rotation, float scale, int sides, bool twoSided);
    ConvexPolyObject(glm::vec3 position, glm::mat4 rotation, float scale, std::vector<glm::vec3> vertices, bool twoSided);

    bool Intersects(Ray& ray, IsectData& isectData);
    bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::mat4 rotation);

    float Scale;
    bool TwoSided;

private:
    void genObjectVertices(int sides);
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
