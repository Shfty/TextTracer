#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <sstream>

#include "DebugBox.h"
#include "WorldObject.h"

class SphereObject : public WorldObject
{
public:
    SphereObject(glm::vec3 position, glm::mat4 rotation, float radius, bool backface);

    bool Intersects(Ray& ray, IsectData& isectData);
    bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);
    bool IntersectsPlane(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation);

    virtual void SetPosition(glm::vec3 position) { m_position = position; }
    virtual void SetRotation(glm::mat4 rotation) { m_rotation = rotation; }

    float Radius = 1.0f;
    bool Backface = false;
};

#endif // SPHEREOBJECT_H
