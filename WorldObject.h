#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "Colour.h"
#include "Ray.h"
#include "IsectData.h"

class WorldObject
{
public:
    Colour ObjectColour = Colours::Black;
    bool Portal = false;
    WorldObject* ExitPortal = NULL;
    WorldObject* ExitDecoration = NULL;

    WorldObject() {}

    glm::vec3 GetPosition() const { return m_position; }
    glm::mat4 GetRotation() const { return m_rotation; }

    virtual void SetPosition(glm::vec3 position) = 0;
    virtual void SetRotation(glm::mat4 rotation) = 0;

    virtual bool Intersects(Ray& ray, IsectData& isectData) = 0;
    virtual bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation) = 0;

protected:
    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::mat4 m_rotation = glm::mat4();
};

#endif // WORLDOBJECT_H
