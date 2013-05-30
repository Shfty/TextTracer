#include "AABBGeometry.h"
#include "Camera.h"
#include "Ray.h"

// PUBLIC
void AABBGeometry::SetPosition(const glm::vec3& position)
{
    m_position = position;
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
    AABB translatedBounds;
    translatedBounds.Min = m_bounds.Min + m_position;
    translatedBounds.Max = m_bounds.Max + m_position;
    if(translatedBounds.Intersects(ray, isectData, camera))
    {
        if(isectData != NULL)
        {
            isectData->EntryNormal = cardinalDirection(m_position - isectData->Entry);
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

glm::vec3 AABBGeometry::cardinalDirection(const glm::vec3& inVector)
{
    float maxIndex = 0;
    float maxVal = 0;
    for(int i = 0; i < 3; i++)
    {
        if(glm::abs(inVector[i]) > maxVal)
        {
            maxVal = glm::abs(inVector[i]);
            maxIndex = i;
        }
    }

    glm::vec3 outVector;
    outVector[maxIndex] = glm::sign(inVector[maxIndex]);

    return outVector;
}
