#include "PlaneObject.h"
#include "Ray.h"

// PUBLIC
PlaneObject::PlaneObject(const glm::vec3& position, const glm::mat4& rotation, const bool twoSided)
    :TwoSided((bool)twoSided)
{
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 1, 1, 1);
}

bool PlaneObject::Intersects(Ray& ray, IsectData& isectData)
{
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !TwoSided)) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;
    else ray.FarPlane = t;

    isectData.Entry = ray.Origin + ray.Direction * t;

    return true;
}

bool PlaneObject::IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    return Intersects(ray, isectData);
}

void PlaneObject::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
}

void PlaneObject::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
}

// PRIVATE
void PlaneObject::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}
