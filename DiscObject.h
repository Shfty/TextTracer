#ifndef DISCOBJECT_H
#define DISCOBJECT_H

#include <vector>
#include "WorldObject.h"

class DiscObject : public WorldObject
{
public:
    DiscObject(glm::vec3 position, glm::mat4 rotation, float scale, bool twoSided);

    bool Intersects(Ray& ray, IsectData& isectData);
    bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);

    void SetPosition(glm::vec3 position);
    void SetRotation(glm::mat4 rotation);

    float Scale;
    bool TwoSided;

private:
    void calculateWorldNormalDotPosition();

    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // DISCOBJECT_H
