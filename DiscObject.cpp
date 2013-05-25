#include "DiscObject.h"

// PUBLIC
DiscObject::DiscObject(glm::vec3 position, glm::mat4 rotation, float scale, bool twoSided)
:TwoSided(twoSided), Scale(scale)
{
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

bool DiscObject::Intersects(Ray& ray, IsectData& isectData)
{
    // Step 1: Plane intersection test
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !TwoSided)) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;

    // Step 2: Circle intersection test
    glm::vec3 pointOnPlaneLocal = pointOnPlane - m_position;
    float distance = glm::dot(pointOnPlaneLocal, pointOnPlaneLocal);
    if(distance > Scale * Scale) return false;

    //Offset portal intersection by a small amount to fake volume
    ray.FarPlane = t - (Portal ? 0.1f : 0.0f);
    isectData.Entry = ray.Origin + ray.Direction * (t - (Portal ? 0.1f : 0.0f));
    return true;
}

bool DiscObject::IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    return Intersects(ray, isectData);
}

void DiscObject::SetPosition(glm::vec3 position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
}

void DiscObject::SetRotation(glm::mat4 rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
}

// PRIVATE
void DiscObject::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}
