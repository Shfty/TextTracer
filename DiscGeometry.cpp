#include "DiscGeometry.h"
#include "Ray.h"

// PUBLIC
void DiscGeometry::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
}

void DiscGeometry::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
}

void DiscGeometry::SetTwoSided(const bool twoSided)
{
    m_twoSided = twoSided;
}

void DiscGeometry::SetScale(const float scale)
{
    m_scale = scale;
}

// PRIVATE
void DiscGeometry::calculateAABB()
{
    // TODO: Implement properly
}

void DiscGeometry::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}

bool DiscGeometry::intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    //if(Parent != NULL && Parent->GetExitPortal() != NULL && Parent->Intersects(ray, NULL)) return false;

    // Step 1: Plane intersection test
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !GetTwoSided())) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;

    // Step 2: Circle intersection test
    glm::vec3 pointOnPlaneLocal = pointOnPlane - m_position;
    float distance = glm::dot(pointOnPlaneLocal, pointOnPlaneLocal);
    if(distance > GetScale() * GetScale()) return false;

    if(isectData != NULL)
    {
        isectData->Distance = t;
        isectData->Entry = ray.Origin + ray.Direction * t;
        isectData->EntryNormal = m_worldNormal * (nDotRay > 0 ? 1.0f : -1.0f);
        isectData->Exit = isectData->Entry;
        isectData->ExitNormal = isectData->EntryNormal;
        isectData->Colour = GetColour();
    }
    return true;
}

bool DiscGeometry::intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    return intersectsGeometry(ray, isectData, camera);
}
