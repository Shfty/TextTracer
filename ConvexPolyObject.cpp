#include "ConvexPolyObject.h"

// PUBLIC
ConvexPolyObject::ConvexPolyObject(glm::vec3 position, glm::mat4 rotation, float scale, int sides, bool twoSided)
:TwoSided(twoSided), Scale(scale)
{
    genObjectVertices(sides);
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

ConvexPolyObject::ConvexPolyObject(glm::vec3 position, glm::mat4 rotation, float scale, std::vector<glm::vec3> vertices, bool twoSided)
:TwoSided(twoSided), Scale(scale)
{
    m_objectVertices = vertices;
    SetPosition(position);
    SetRotation(rotation);
    ObjectColour = glm::vec4(1, 0, 0, 1);
}

bool ConvexPolyObject::Intersects(Ray& ray, IsectData& isectData)
{
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

    //Offset portal intersection by a small amount to fake volume
    ray.FarPlane = t - (Portal ? 0.1f : 0.0f);
    isectData.Entry = ray.Origin + ray.Direction * (t - (Portal ? 0.1f : 0.0f));
    return true;
}

bool ConvexPolyObject::IntersectsPortal(Ray& ray, IsectData& isectData, const glm::mat4& cameraRotation)
{
    return Intersects(ray, isectData);
}

void ConvexPolyObject::SetPosition(glm::vec3 position)
{
    m_position = position;
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

void ConvexPolyObject::SetRotation(glm::mat4 rotation)
{
    m_rotation = rotation;
    m_worldNormal = glm::normalize(glm::vec3(m_rotation * glm::vec4(m_objectNormal, 1.0f)));
    calculateWorldNormalDotPosition();
    genWorldVertices();
    genWorldEdges();
}

// PRIVATE
void ConvexPolyObject::genObjectVertices(int sides)
{
    //Clamp sides to prevent <2D objects
    sides = glm::max(sides, 3);

    //Create radius and rotate so bottom edge is perpendicular to 1, 0, 0
    glm::vec3 vertex(0, 1, 0);
    vertex = glm::rotateZ(vertex, 180.0f / (float)sides);

    //For each side, add a vertex and rotate
    for(int i = 0; i < sides; i++)
    {
        m_objectVertices.push_back(vertex);
        vertex = glm::rotateZ(vertex, 360.0f / (float)sides);
    }
}

void ConvexPolyObject::genWorldVertices()
{
    m_worldVertices.clear();
    for(int i = 0; i < m_objectVertices.size(); i++)
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
