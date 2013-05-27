#include "ConvexPolyObject.h"
#include "Ray.h"

// PUBLIC
ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const int sides, const bool twoSided)
:Scale((float)scale), TwoSided((bool)twoSided)
{
    genObjectVertices(sides);
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const std::vector<glm::vec3>& vertices, const bool twoSided)
:Scale((float)scale), TwoSided((bool)twoSided)
{
    m_objectVertices = vertices;
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

bool ConvexPolyObject::Intersects(Ray& ray, IsectData& isectData)
{
    IsectData dummy;
    if(Parent != NULL && Parent->GetExitPortal() != NULL && Parent->Intersects(ray, dummy)) return false;

    // Step 1: Plane intersection test
    float nDotRay = glm::dot(m_worldNormal, ray.Direction);
    if(nDotRay == 0 || (nDotRay > 0 && !TwoSided)) return false;

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

    ray.FarPlane = t;
    isectData.Entry = pointOnPlane;
    return true;
}

bool ConvexPolyObject::IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    return Intersects(ray, isectData);
}

void ConvexPolyObject::SetPosition(const glm::vec3& position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyObject::SetRotation(const glm::mat4& rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

// PRIVATE
void ConvexPolyObject::genObjectVertices(const int sides)
{
    //Clamp sides to prevent <2D objects
    int clampSides = glm::max(sides, 3);

    //Create radius and rotate so bottom edge is perpendicular to 1, 0, 0
    glm::vec3 vertex(0, 1, 0);
    vertex = glm::rotateZ(vertex, 180.0f / (float)clampSides);

    //For each side, add a vertex and rotate
    for(int i = 0; i < clampSides; i++)
    {
        m_objectVertices.push_back(vertex);
        vertex = glm::rotateZ(vertex, 360.0f / (float)clampSides);
    }
}

void ConvexPolyObject::genWorldVertices()
{
    m_worldVertices.clear();
    for(uint16_t i = 0; i < m_objectVertices.size(); i++)
    {
        m_worldVertices.push_back(glm::vec3(m_rotation * glm::vec4(m_objectVertices[i] * Scale, 1.0f)) + m_position);
    }
}

void ConvexPolyObject::genWorldEdges()
{
    m_worldEdges.clear();
    for(uint16_t i = 0; i < m_worldVertices.size(); i++)
    {
        glm::vec3 v0 = m_worldVertices[i];
        glm::vec3 v1 = m_worldVertices[i < m_worldVertices.size() - 1 ? i + 1 : 0];

        m_worldEdges.push_back(glm::normalize(v1 - v0));
    }
}

void ConvexPolyObject::calculateWorldNormalDotPosition()
{
    m_worldNormalDotPosition = glm::dot(m_worldNormal, m_position);
}
