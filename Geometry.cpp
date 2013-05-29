#include "Geometry.h"

bool Geometry::Intersects(const Ray& ray, IsectData* isectData, const Camera* camera, const bool portal)
{
    if(m_csgSubtract != NULL && m_csgSubtract->Intersects(ray, isectData, camera, portal)) return false;

    if(UseBoundingBox)
    {
        if(intersectsAABB(ray))
        {
            return portal ? intersectsPortal(ray, isectData, camera) : intersectsGeometry(ray, isectData, camera);
        }
    }

    return portal ? intersectsPortal(ray, isectData, camera) : intersectsGeometry(ray, isectData, camera);
}

bool Geometry::intersectsAABB(const Ray& ray)
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
