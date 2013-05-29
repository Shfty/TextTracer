#include "WorldObject.h"
#include "IsectData.h"

void WorldObject::AddChild(WorldObject* child)
{
    child->Parent = this; m_children.push_back(child);
}

bool WorldObject::Intersects(const Ray& ray, IsectData* isectData, const Camera* camera, const bool portal)
{
    if(m_geometry->Intersects(ray, isectData, camera, portal))
    {
        if(isectData != NULL)
        {
            isectData->Object = this;
        }
        return true;
    }

    return false;
}
