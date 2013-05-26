#ifndef PLANEOBJECT_H
#define PLANEOBJECT_H

#include "WorldObject.h"

class PlaneObject : public WorldObject
{
public:
    PlaneObject(const glm::vec3& position, const glm::mat4& rotation, bool twoSided);

    bool Intersects(Ray& ray, IsectData& isectData);
    bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation);

    bool TwoSided = false;

private:
    void calculateWorldNormalDotPosition();

    glm::vec3 m_objectNormal = glm::vec3(0, 0, 1);
    glm::vec3 m_worldNormal = glm::vec3(0, 0, 1);
    float m_worldNormalDotPosition = 0;
};

#endif // PLANEOBJECT_H
