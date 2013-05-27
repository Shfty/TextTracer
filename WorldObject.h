#ifndef WORLDOBJECT_H
#define WORLDOBJECT_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "IsectData.h"
#include <vector>

class Ray;

class WorldObject
{
public:
    WorldObject* Parent = NULL;

    glm::vec4 ObjectColour = glm::vec4(0, 0, 0, 1);

    bool CastShadow = true;
    bool Fullbright = false;

    WorldObject() {}
    virtual ~WorldObject() {};

    glm::vec3 GetPosition() const { return m_position; }
    glm::mat4 GetRotation() const { return m_rotation; }
    WorldObject* GetExitPortal() const { return m_exitPortal; }
    std::vector<WorldObject*> GetChildren() const { return m_children; }
    void AddChild(WorldObject* child) { child->Parent = this; m_children.push_back(child); }

    virtual void SetPosition(const glm::vec3& position) = 0;
    virtual void SetRotation(const glm::mat4& rotation) = 0;
    void SetExitPortal(const WorldObject* exitPortal) { m_exitPortal = (WorldObject*)exitPortal; }

    virtual bool Intersects(Ray& ray, IsectData& isectData) = 0;
    virtual bool IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation) = 0;

protected:
    std::vector<WorldObject*> m_children;
    WorldObject* m_exitPortal = NULL;

    glm::vec3 m_position = glm::vec3(0, 0, 0);
    glm::mat4 m_rotation = glm::mat4();
};

#endif // WORLDOBJECT_H
