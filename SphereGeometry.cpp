#include "SphereGeometry.h"
#include "Ray.h"
#include "Camera.h"

// PUBLIC
void SphereGeometry::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateAABB();
}

void SphereGeometry::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
}

void SphereGeometry::SetRadius(const float radius)
{
    m_radius = radius;
    calculateAABB();
}

void SphereGeometry::SetBackface(const bool backface)
{
    m_backface = backface;
}

// PRIVATE
void SphereGeometry::calculateAABB()
{
    glm::vec3 minBound = glm::vec3(-m_radius) + m_position;
    glm::vec3 maxBound = glm::vec3(m_radius) + m_position;
    m_bounds = AABB(minBound, maxBound);
}

bool SphereGeometry::intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    float t0;
    float t1;
    float radius2 = m_radius * m_radius;

    // geometric solution
    glm::vec3 L = m_position - ray.Origin;
    float tca = glm::dot(L, ray.Direction);
    if (tca < 0) return false;
    float d2 = glm::dot(L, L) - tca * tca;
    if (d2 > radius2) return false;
    float thc = sqrt(radius2 - d2);
    t0 = tca - thc; //Entering sphere
    t1 = tca + thc; //Leaving sphere

    if(!m_backface)
    {
            if (t0 > ray.FarPlane) return false;
            if(isectData != NULL)
            {
                isectData->Distance = t0;
                isectData->Entry = ray.Origin + ray.Direction * t0;
                isectData->Exit = ray.Origin + ray.Direction * t1;
            }
    }
    else
    {
            if (t1 > ray.FarPlane) return false;
            if(isectData != NULL)
            {
                isectData->Distance = t1;
                isectData->Entry = ray.Origin + ray.Direction * t1;
                isectData->Exit = isectData->Entry;
            }
    }

    if(isectData != NULL)
    {
        isectData->Colour = m_colour; // Portal Colour
    }

    return true;
}

bool SphereGeometry::intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    if(!intersectsAABB(ray)) return false;

    glm::vec3 rNormal = glm::vec3(camera->GetRotation() * glm::vec4(0, 0, 1, 1.0f));

    float nDotRay = glm::dot(rNormal, ray.Direction);
    if(nDotRay == 0) return false;

    float D = glm::dot(rNormal, m_position);
    float t = - (glm::dot(rNormal, ray.Origin) - D) / glm::dot(rNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;

    // Step 2: Circle intersection test
    glm::vec3 pointOnPlaneLocal = pointOnPlane - m_position;
    float distance = glm::dot(pointOnPlaneLocal, pointOnPlaneLocal);
    if(distance > m_radius * m_radius) return false;

    if(isectData != NULL)
    {
        isectData->Distance = t;
        isectData->Entry = ray.Origin + ray.Direction * t;
        isectData->Exit = isectData->Entry;
    }
    return true;
}
