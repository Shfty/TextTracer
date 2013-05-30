#include "PlaneObject.h"
#include "PlaneGeometry.h"

// PUBLIC
PlaneObject::PlaneObject(const glm::vec3& position, const glm::mat4& rotation, const bool twoSided)
{
    m_geometry = new PlaneGeometry();
    SetPosition(position);
    SetRotation(rotation);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 1, 1, 1));
}

PlaneObject::PlaneObject(const glm::vec3& position, const glm::vec3& normal, const bool twoSided)
{
    m_geometry = new PlaneGeometry();
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

    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 1, 1, 1));
}
