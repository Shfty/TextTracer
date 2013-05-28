#include "WorldObject.h"

void WorldObject::AddChild(WorldObject* child)
{
    child->Parent = this; m_children.push_back(child);
}

bool WorldObject::Intersects(const Ray& ray, IsectData* isectData)
{
    if(UseBoundingBox)
    {
        if(intersectsAABB(ray))
        {
            return intersectsGeometry(ray, isectData);
        }
    }
    else
    {
        return intersectsGeometry(ray, isectData);
    }
}

bool WorldObject::intersectsAABB(const Ray& ray)
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
