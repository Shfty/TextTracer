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
