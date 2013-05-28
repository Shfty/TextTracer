#include "DiscObject.h"
#include "Ray.h"

// PUBLIC
DiscObject::DiscObject(const glm::vec3& position, const glm::mat4& rotation, float scale, bool twoSided)
:Scale(scale), TwoSided(twoSided)
{
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

bool DiscObject::IntersectsPortal(const Ray& ray, IsectData* isectData, const glm::mat4& cameraRotation)
{
    if(UseBoundingBox)
    {
        if(intersectsAABB(ray))
        {
            return intersectsGeometry(ray, isectData);
        }
    }

    return intersectsGeometry(ray, isectData);
}

void DiscObject::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
}

void DiscObject::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
}

// PRIVATE
void DiscObject::calculateAABB()
{
    // TODO: Implement properly
}

bool DiscObject::intersectsGeometry(const Ray& ray, IsectData* isectData)
{
    if(Parent != NULL && Parent->GetExitPortal() != NULL && Parent->Intersects(ray, NULL)) return false;

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

void DiscObject::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}
