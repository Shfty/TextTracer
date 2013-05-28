#ifndef SPHEREOBJECT_H
#define SPHEREOBJECT_H

#include <sstream>

#include "DebugBox.h"
#include "WorldObject.h"

class SphereObject : public WorldObject
{
public:
    SphereObject(const glm::vec3& position, const glm::mat4& rotation, const float radius, const bool backface);

    bool IntersectsPortal(const Ray& ray, IsectData* isectData, const glm::mat4& cameraRotation);

    void SetPosition(const glm::vec3& position);
    void SetRotation(const glm::mat4& rotation) { m_rotation = rotation; }
    void SetRadius(const float radius);

    bool Backface = false;

private:
    void calculateAABB();
    bool intersectsGeometry(const Ray& ray, IsectData* isectData);

    float m_radius = 1.0f;
};

#endif // SPHEREOBJECT_H
