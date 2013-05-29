#include "SphereObject.h"
#include "Ray.h"

// PUBLIC
SphereObject::SphereObject(const glm::vec3& position, const glm::mat4& rotation, const float radius, const bool backface)
{
    m_geometry = new SphereGeometry();
    SetPosition(position);
    SetRotation(rotation);
    SetRadius(radius);
    SetBackface(backface);
    SetColour(glm::vec4(1, 0, 0, 1));
}

// PRIVATE
