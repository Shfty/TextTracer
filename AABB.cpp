#include "AABB.h"

#include "Ray.h"
#include "Camera.h"

bool AABB::Contains(const glm::vec3& point)
{
    bool contains = true;
    for(int i = 0; i < 3; i++)
    {
        if(point[i] < Min[i] || point[i] > Max[i])
        {
            contains = false;
        }
    }
    return contains;
}

// Simple yes/no intersection for broad-phase collision
bool AABB::Intersects(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    // EZ cases: if the ray starts inside the box, or ends inside
    // the box, then it definitely hits the box.
    // I'm using this code for ray tracing with an octree,
    // so I needed rays that start and end within an
    // octree node to COUNT as hits.
    // You could modify this test to (ray starts inside and ends outside)
    // to qualify as a hit if you wanted to NOT count totally internal rays
    if( Contains( ray.Origin ) )
    {
        if(isectData != NULL)
        {
            isectData->Distance = 0;
            isectData->Entry = ray.Origin;
            isectData->EntryNormal = glm::vec3(0, 0, 1);
            isectData->Exit = isectData->Entry;
            isectData->ExitNormal = isectData->EntryNormal;
        }

        return true ;
    }

    // the algorithm says, find 3 t's,
    glm::vec3 t ;

    // LARGEST t is the only one we need to test if it's on the face.
    for( int i = 0 ; i < 3 ; i++ )
    {
        if( ray.Direction[i] > 0 ) // CULL BACK FACE
          t[i] = ( Min[i] - ray.Origin[i] ) / ray.Direction[i] ;
        else
          t[i] = ( Max[i] - ray.Origin[i] ) / ray.Direction[i] ;
    }

    int mi = 0;
    float val = 0;
    for(int i = 0; i < 3; i++)
    {
        float indexVal = t[i];
        if(indexVal > val)
        {
            val = indexVal;
            mi = i;
        }
    }

    if(t[mi] < ray.NearPlane || t[mi] > ray.FarPlane) return false;

    glm::vec3 pt = ray.Origin + ( ray.Direction * t[mi] ) ;

    // check it's in the box in other 2 dimensions
    int o1 = ( mi + 1 ) % 3 ; // i=0: o1=1, o2=2, i=1: o1=2,o2=0 etc.
    int o2 = ( mi + 2 ) % 3 ;

    if((pt[o1] > Min[o1] && pt[o1] < Max[o1]) && (pt[o2] > Min[o2] && pt[o2] < Max[o2]))
    {
        if(isectData != NULL)
        {
            isectData->Distance = t[mi];
            isectData->Entry = pt;
            isectData->Exit = isectData->Entry;
        }
        return true;
    }

    return false ; // the ray did not hit the box.
}
