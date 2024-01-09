#include "AABB.h"

#include "Ray.h"
#include "Camera.h"

AABB::AABB(const glm::vec3& minBound, const glm::vec3& maxBound)
{
    m_minBoundLocal = minBound;
    m_maxBoundLocal = maxBound;
    SetPosition(glm::vec3());
}

AABB::AABB(const glm::vec3& position, const glm::vec3& minBound, const glm::vec3& maxBound)
{
    m_minBoundLocal = minBound;
    m_maxBoundLocal = maxBound;
    SetPosition(position);
}

void AABB::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldBounds();
}

void AABB::SetMin(const glm::vec3& minBound)
{
    m_minBoundLocal = minBound;
    calculateWorldBounds();
}

void AABB::SetMax(const glm::vec3& maxBound)
{
    m_maxBoundLocal = maxBound;
    calculateWorldBounds();
}

bool AABB::Contains(const glm::vec3& point)
{
    bool contains = true;
    for(int i = 0; i < 3; i++)
    {
        if(point[i] < m_minBoundWorld[i] || point[i] > m_maxBoundWorld[i])
        {
            contains = false;
        }
    }
    return contains;
}

bool AABB::Intersects(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    if(Contains( ray.Origin ))
    {
        if(isectData != NULL)
        {
            isectData->EntryDistance = 0;
            isectData->Entry = ray.Origin;
            isectData->EntryNormal = -glm::normalize(ray.Direction);
            isectData->ExitDistance = isectData->EntryDistance;
            isectData->Exit = isectData->Entry;
            isectData->ExitNormal = -isectData->EntryNormal;
        }
        return true;
    }
    bool backface = m_backface || Contains( ray.Origin );

    // find 3 ts (one per axis) for the near and far intersections
    glm::vec3 nt;
    glm::vec3 ft;

    // LARGEST t is the only one we need to test if it's on the face.
    for( int i = 0 ; i < 6 ; i++ )
    {
        if(i < 3)
        {
            if( ray.Direction[i] > 0 ) // CULL BACK FACE
              nt[i] = ( m_minBoundWorld[i] - ray.Origin[i] ) / ray.Direction[i] ;
            else
              nt[i] = ( m_maxBoundWorld[i] - ray.Origin[i] ) / ray.Direction[i] ;
        }
        else
        {
            if( ray.Direction[i - 3] > 0 ) // CULL BACK FACE
              ft[i - 3] = ( m_maxBoundWorld[i - 3] - ray.Origin[i - 3] ) / ray.Direction[i - 3] ;
            else
              ft[i - 3] = ( m_minBoundWorld[i - 3] - ray.Origin[i - 3] ) / ray.Direction[i - 3] ;
        }
    }

    int miNear = 0;
    float valNear = 0;
    int miFar = 0;
    float valFar = 0;

    for(int i = 0; i < 6; i++)
    {
        if(i < 3)
        {
            float indexValNear = nt[i];
            if(i == 0)
            {
                valNear = indexValNear;
            }
            else
            {
                if(indexValNear > valNear)
                {
                    valNear = indexValNear;
                    miNear = i;
                }
            }
        }
        else
        {
            float indexValFar = ft[i - 3];
            if(i - 3 == 0)
            {
                valFar = indexValFar;
            }
            else
            {
                if(indexValFar < valFar)
                {
                    valFar = indexValFar;
                    miFar = i - 3;
                }
            }
        }
    }

    if(!backface)
    {
        if(nt[miNear] < ray.NearPlane || nt[miNear] > ray.FarPlane) return false;
    }
    else
    {
        if(ft[miFar] < ray.NearPlane || ft[miFar] > ray.FarPlane) return false;
    }

    glm::vec3 ptNear = ray.Origin + ( ray.Direction * nt[miNear] ) ;
    glm::vec3 ptFar = ray.Origin + ( ray.Direction * ft[miNear] ) ;

    // check it's in the box in other 2 dimensions

    if(!backface)
    {
        int o1 = ( miNear + 1 ) % 3 ; // i=0: o1=1, o2=2, i=1: o1=2,o2=0 etc.
        int o2 = ( miNear + 2 ) % 3 ;

        if(
            (ptNear[o1] < m_minBoundWorld[o1] || ptNear[o1] > m_maxBoundWorld[o1]) ||
            (ptNear[o2] < m_minBoundWorld[o2] || ptNear[o2] > m_maxBoundWorld[o2])
        ) return false;
    }
    else
    {
        int o1 = ( miFar + 1 ) % 3 ; // i=0: o1=1, o2=2, i=1: o1=2,o2=0 etc.
        int o2 = ( miFar + 2 ) % 3 ;

        if(
            (ptFar[o1] < m_minBoundWorld[o1] || ptFar[o1] > m_maxBoundWorld[o1]) ||
            (ptFar[o2] < m_minBoundWorld[o2] || ptFar[o2] > m_maxBoundWorld[o2])
        ) return false;
    }

    if(isectData != NULL)
    {
        isectData->EntryDistance = nt[miNear];
        isectData->Entry = ptNear;
        isectData->EntryNormal = cardinalDirection(m_position - isectData->Entry);
        isectData->ExitDistance = ft[miFar];
        isectData->Exit = ptFar;
        isectData->ExitNormal = -cardinalDirection(m_position - isectData->Entry);
    }

    return true ; // the ray hit the box.
}

// PRIVATE
void AABB::calculateWorldBounds()
{
    m_minBoundWorld = m_minBoundLocal + m_position;
    m_maxBoundWorld = m_maxBoundLocal + m_position;
}

glm::vec3 AABB::cardinalDirection(const glm::vec3& inVector)
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
