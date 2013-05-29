#include "WorldObject.h"
#include "IsectData.h"

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
