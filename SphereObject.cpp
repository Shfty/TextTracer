#include "SphereObject.h"
#include "Ray.h"

// PUBLIC
SphereObject::SphereObject(const glm::vec3& position, const glm::mat4& rotation, const float radius, const bool backface)
    : Radius(radius),
    Backface(backface)
{
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

bool SphereObject::Intersects(Ray& ray, IsectData& isectData)
{
    float t0;
    float t1;
    float radius2 = Radius * Radius;

    // geometric solution
    glm::vec3 L = m_position - ray.Origin;
    float tca = glm::dot(L, ray.Direction);
    if (tca < 0) return false;
    float d2 = glm::dot(L, L) - tca * tca;
    if (d2 > radius2) return false;
    float thc = sqrt(radius2 - d2);
    t0 = tca - thc; //Entering sphere
    t1 = tca + thc; //Leaving sphere

    if(!Backface)
    {
            if (t0 > ray.FarPlane) return false;
            else ray.FarPlane = t0;
            isectData.Entry = ray.Origin + ray.Direction * t0;
            isectData.Exit = ray.Origin + ray.Direction * t1;
    }
    else
    {
            if (t1 > ray.FarPlane) return false;
            else ray.FarPlane = t1;
            isectData.Entry = ray.Origin + ray.Direction * t1;
    }
    return true;
}

bool SphereObject::IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    glm::vec3 rNormal = glm::vec3(cameraRotation * glm::vec4(0, 0, 1, 1.0f));

    float nDotRay = glm::dot(rNormal, ray.Direction);
    if(nDotRay == 0) return false;

    float D = glm::dot(rNormal, m_position);
    float t = - (glm::dot(rNormal, ray.Origin) - D) / glm::dot(rNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;

    // Step 2: Circle intersection test
    glm::vec3 pointOnPlaneLocal = pointOnPlane - m_position;
    float distance = glm::dot(pointOnPlaneLocal, pointOnPlaneLocal);
    if(distance > Radius * Radius) return false;

    //Offset portal intersection by a small amount to fake volume
    ray.FarPlane = t - (Portal ? 0.1f : 0.0f);
    isectData.Entry = ray.Origin + ray.Direction * (t - (Portal ? 0.1f : 0.0f));
    return true;
}

// PRIVATE
