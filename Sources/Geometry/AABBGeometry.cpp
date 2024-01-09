#include "AABBGeometry.h"
#include "Camera.h"
#include "Ray.h"

// PUBLIC
void AABBGeometry::SetPosition(const glm::vec3& position)
{
    m_bounds.SetPosition(position);
}

void AABBGeometry::SetRotation(const glm::mat4& rotation)
{
    // No implementaion since AABBs don't rotate
}

void AABBGeometry::SetBounds(const AABB& bounds)
{
    m_bounds = bounds;
}

// PRIVATE
void AABBGeometry::calculateAABB()
{
    // No implementation because of SetBounds
}

bool AABBGeometry::intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    if(m_bounds.Intersects(ray, isectData, camera))
    {
        if(isectData != NULL)
        {
            isectData->Colour = m_colour;
        }
        return true;
    }

    return false;
}

bool AABBGeometry::intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    return intersectsGeometry(ray, isectData, camera);
}
