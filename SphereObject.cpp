#include "SphereObject.h"

// PUBLIC
SphereObject::SphereObject(glm::vec3 position, glm::mat4 rotation, float radius, bool backface)
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
        IsectData dummy;
        return IntersectsPlane(ray, isectData, cameraRotation) && Intersects(ray, dummy);
}

bool SphereObject::IntersectsPlane(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    glm::vec3 rNormal = glm::vec3(cameraRotation * glm::vec4(0, 0, 1, 1.0f));

    float nDotRay = glm::dot(rNormal, ray.Direction);
    if(nDotRay = 0) return false;

    float D = glm::dot(rNormal, m_position);
    float t = - (glm::dot(rNormal, ray.Origin) - D) / glm::dot(rNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;
    else ray.FarPlane = t;

    isectData.Entry = ray.Origin + ray.Direction * t;
    return true;
}

// PRIVATE
