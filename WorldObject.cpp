#include "WorldObject.h"

void WorldObject::AddChild(WorldObject* child)
{
    child->Parent = this; m_children.push_back(child);
}

bool WorldObject::Intersects(Ray& ray, IsectData& isectData)
{
    return intersectsAABB(ray) && intersectsGeometry(ray, isectData);
}

bool WorldObject::intersectsAABB(Ray& ray)
{
    // If the object has a bounding box of size 0 (aka no bounding box/disabled), return true
    if(m_bounds.Min == glm::vec3() && m_bounds.Max == glm::vec3())
    {
        return true;
    }
    else
    {
        return m_bounds.Intersects(ray);
    }
}
