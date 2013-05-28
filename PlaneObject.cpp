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

bool PlaneObject::IntersectsPortal(const Ray& ray, IsectData* isectData, const glm::mat4& cameraRotation)
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
void PlaneObject::calculateAABB()
{
    // No implementation since planes are infinite
}

bool PlaneObject::intersectsGeometry(const Ray& ray, IsectData* isectData)
{
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !TwoSided)) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    if(isectData != NULL)
    {
        isectData->Distance = t;
        isectData->Entry = ray.Origin + ray.Direction * t;
        isectData->Exit = ray.Origin + ray.Direction * t;
        isectData->Colour = ObjectColour;
        isectData->Object = this;
    }
    return true;
}

void PlaneObject::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}
