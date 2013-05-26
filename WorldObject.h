#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "IsectData.h"

class Ray;

class WorldObject
{
public:
    WorldObject* ExitPortal = NULL;
    WorldObject* ExitDecoration = NULL;

    glm::vec4 ObjectColour = glm::vec4(0, 0, 0, 1);

    bool Portal = false;
    bool CastShadow = true;
    bool Fullbright = false;

    WorldObject() {}
    virtual ~WorldObject() {};

    glm::vec3 GetPosition() const { return m_position; }
    glm::mat4 GetRotation() const { return m_rotation; }

    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual void SetRotation(const glm::mat4& rotation) = 0;

    virtual bool Intersects(Ray& ray, IsectData& isectData) = 0;
    virtual bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation) = 0;

protected:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::mat4 m_rotation = glm::mat4();
};

#endif // WORLDOBJECT_H
