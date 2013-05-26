#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <sstream>

#include "DebugBox.h"
#include "WorldObject.h"

class SphereObject : public WorldObject
{
public:
    SphereObject(const glm::vec3& position, const glm::mat4& rotation, const float radius, const bool backface);

    bool Intersects(Ray& ray, IsectData& isectData);
    bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);
    bool IntersectsPlane(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);

    virtual void SetPosition(const glm::vec3& position) { m_position = position; }
    virtual void SetRotation(const glm::mat4& rotation) { m_rotation = rotation; }

    float Radius = 1.0f;
    bool Backface = false;
};

#endif // SPHEREOBJECT_H
