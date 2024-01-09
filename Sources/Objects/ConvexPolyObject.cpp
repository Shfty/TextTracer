#include "ConvexPolyObject.h"
#include "Ray.h"

#include <glm/gtx/transform.hpp>

// PUBLIC
ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const int sides, const bool twoSided)
{
    m_geometry = new ConvexPolyGeometry();
    SetSides(sides);
    SetPosition(position);
    SetRotation(rotation);
    SetScale(scale);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 0, 0, 1));
}

ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::vec3& normal, const float scale, const int sides, const bool twoSided)
{
    m_geometry = new ConvexPolyGeometry();
    SetSides(sides);
    SetPosition(position);

    glm::vec3 backward = glm::vec3(0, 0, 1);
    glm::vec3 normalizedNormal = glm::normalize(normal);
    if(normalizedNormal != backward)
    {
        glm::vec3 axis = glm::cross(backward, normalizedNormal);
        float angle = glm::acos(glm::dot(backward, normalizedNormal));
        glm::mat4 rotation = glm::rotate(glm::degrees(angle), axis);
        SetRotation(rotation);
    }

    SetScale(scale);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 0, 0, 1));
}

ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::mat4& rotation, const float scale, const std::vector<glm::vec3>& vertices, const bool twoSided)
{
    m_geometry = new ConvexPolyGeometry();
    SetObjectVertices(vertices);
    SetPosition(position);
    SetRotation(rotation);
    SetScale(scale);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 0, 0, 1));
}

ConvexPolyObject::ConvexPolyObject(const glm::vec3& position, const glm::vec3& normal, const float scale, const std::vector<glm::vec3>& vertices, const bool twoSided)
{
    m_geometry = new ConvexPolyGeometry();
    SetObjectVertices(vertices);
    SetPosition(position);

    glm::vec3 backward = glm::vec3(0, 0, 1);
    glm::vec3 normalizedNormal = glm::normalize(normal);
    if(normalizedNormal != backward)
    {
        glm::vec3 axis = glm::cross(backward, normalizedNormal);
        float angle = glm::acos(glm::dot(backward, normalizedNormal));
        glm::mat4 rotation = glm::rotate(glm::degrees(angle), axis);
        SetRotation(rotation);
    }

    SetScale(scale);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 0, 0, 1));
}

// PRIVATE

