#include "ConvexPolyGeometry.h"

#include <glm/gtx/euler_angles.hpp>

#include "Ray.h"

// PUBLIC
void ConvexPolyGeometry::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyGeometry::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyGeometry::SetScale(const float scale)
{
    m_scale = scale;
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyGeometry::SetTwoSided(const bool twoSided)
{
    m_twoSided = twoSided;
}

void ConvexPolyGeometry::SetSides(const int sides)
{
    m_sides = sides;
    genObjectVertices();
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyGeometry::SetObjectVertices(const std::vector<glm::vec3>& objectVertices)
{
    m_objectVertices = objectVertices;
    genWorldVertices();
    genWorldEdges();
}

// PRIVATE
void ConvexPolyGeometry::calculateAABB()
{
    glm::vec3 minBound = m_objectVertices[0];
    glm::vec3 maxBound = m_objectVertices[0];

    for(uint16_t i = 0; i < m_objectVertices.size(); i++)
    {
        for(uint16_t o = 0; o < 3; o++)
        {
            if(m_objectVertices[i][o] < minBound[o])
            {
                minBound[o] = m_objectVertices[i][o];
            }

            if(m_objectVertices[i][o] > maxBound[o])
            {
                maxBound[o] = m_objectVertices[i][o];
            }
        }
    }

    m_bounds = AABB(m_position, minBound, maxBound);
}

void ConvexPolyGeometry::genObjectVertices()
{
    //Clamp sides to prevent <2D objects
    int clampSides = glm::max(m_sides, 3);

    //Create radius and rotate so bottom edge is perpendicular to 1, 0, 0
    glm::vec4 vertex(0, 1, 0, 0);
    vertex = glm::eulerAngleZ(3.14159 / (float)clampSides) * vertex;

    //For each side, add a vertex and rotate
    for(int i = 0; i < clampSides; i++)
    {
        m_objectVertices.push_back(glm::vec3(vertex.x, vertex.y, vertex.z));
        vertex = glm::eulerAngleZ((3.14159 * 2.0) / (float)clampSides) * vertex;
    }
}

void ConvexPolyGeometry::genWorldVertices()
{
    m_worldVertices.clear();
    for(uint16_t i = 0; i < m_objectVertices.size(); i++)
    {
        m_worldVertices.push_back(glm::vec3(m_rotation * glm::vec4(m_objectVertices[i] * m_scale, 1.0f)) + m_position);
    }

    calculateAABB();
}

void ConvexPolyGeometry::genWorldEdges()
{
    m_worldEdges.clear();
    for(uint16_t i = 0; i < m_worldVertices.size(); i++)
    {
        glm::vec3 v0 = m_worldVertices[i];
        glm::vec3 v1 = m_worldVertices[i < m_worldVertices.size() - 1 ? i + 1 : 0];

        m_worldEdges.push_back(glm::normalize(v1 - v0));
    }
}

void ConvexPolyGeometry::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}

bool ConvexPolyGeometry::intersectsGeometry(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    //if(Parent != NULL && Parent->GetExitPortal() != NULL && Parent->Intersects(ray, (IsectData*)NULL)) return false;

    // Step 1: Plane intersection test
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !m_twoSided)) return false;

    float t = - (glm::dot(m_worldNormal, ray.Origin) - m_worldNormalDotPosition) / glm::dot(m_worldNormal, ray.Direction);

    if(t <= ray.NearPlane || t > ray.FarPlane) return false;

    glm::vec3 pointOnPlane = ray.Origin + ray.Direction * t;

    // Step 2: Polygon intersection test
    for(uint16_t i = 0; i < m_worldVertices.size(); i++)
    {
        glm::vec3 edge = m_worldEdges[i];
        glm::vec3 C = glm::normalize(pointOnPlane - m_worldVertices[i]);

        if(glm::dot(m_worldNormal, glm::cross(edge, C)) < 0) return false;
    }

    if(isectData != NULL)
    {
        isectData->EntryDistance = t;
        isectData->Entry = ray.Origin + ray.Direction * t;
        isectData->EntryNormal = m_worldNormal * (nDotRay > 0 ? 1.0f : -1.0f);
        isectData->ExitDistance = isectData->EntryDistance;
        isectData->Exit = isectData->Entry;
        isectData->ExitNormal = isectData->EntryNormal;
        isectData->Colour = m_colour;
    }
    return true;
}

bool ConvexPolyGeometry::intersectsPortal(const Ray& ray, IsectData* isectData, const Camera* camera)
{
    return intersectsGeometry(ray, isectData, camera);
}
