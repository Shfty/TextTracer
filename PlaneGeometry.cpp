#include "PlaneGeometry.h"
#include "Camera.h"
#include "Ray.h"

// PUBLIC
void PlaneGeometry::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
}

void PlaneGeometry::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
}

void PlaneGeometry::SetTwoSided(const bool twoSided)
{
    m_twoSided = twoSided;
}

// PRIVATE
void PlaneGeometry::calculateAABB()
{
    // No implementation since planes are infinite
}

void PlaneGeometry::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}

bool PlaneGeometry::intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && m_twoSided)) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    if(isectData != NULL)
    {
        isectData->Distance = t;
        isectData->Entry = ray.Origin + ray.Direction * t;
        isectData->Exit = ray.Origin + ray.Direction * t;
        isectData->Colour = m_colour;
    }
    return true;
}

bool PlaneGeometry::intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    if(!intersectsAABB(ray)) return false;

    glm::vec3 rNormal = glm::vec3(camera->GetRotation() * glm::vec4(0, 0, 1, 1.0f));

    float nDotRay = glm::dot(rNormal, ray.Direction);
    if(nDotRay == 0) return false;

    float D = glm::dot(rNormal, m_position);
    float t = - (glm::dot(rNormal, ray.Origin) - D) / glm::dot(rNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    //glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;
    return true;
}
