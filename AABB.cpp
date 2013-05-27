#include "AABB.h"

#include "Ray.h"

bool AABB::Intersects(Ray& ray)
{
    glm::vec3 dirfrac;

    // r.dir is unit direction vector of ray
    dirfrac.x = 1.0f / ray.Direction.x;
    dirfrac.y = 1.0f / ray.Direction.y;
    dirfrac.z = 1.0f / ray.Direction.z;
    // lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
    // r.org is origin of ray
    float t1 = (Min.x - ray.Origin.x)*dirfrac.x;
    float t2 = (Max.x - ray.Origin.x)*dirfrac.x;
    float t3 = (Min.y - ray.Origin.y)*dirfrac.y;
    float t4 = (Max.y - ray.Origin.y)*dirfrac.y;
    float t5 = (Min.z - ray.Origin.z)*dirfrac.z;
    float t6 = (Max.z - ray.Origin.z)*dirfrac.z;

    float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
    float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

    // if tmax < 0, ray (line) is intersecting AABB, but whole AABB is behing us
    if (tmax < 0)
    {
        //t = tmax;
        return false;
    }

    // if tmin > tmax, ray doesn't intersect AABB
    if (tmin > tmax)
    {
        //t = tmax;
        return false;
    }

    //t = tmin;
    return true;
}
