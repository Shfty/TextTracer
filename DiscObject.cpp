#include "DiscObject.h"
#include "DiscGeometry.h"
#include "Ray.h"

// PUBLIC
DiscObject::DiscObject(const glm::vec3& position, const glm::mat4& rotation, float scale, bool twoSided)
{
    m_geometry = new DiscGeometry();
    SetPosition(position);
    SetRotation(rotation);
    SetScale(scale);
    SetTwoSided(twoSided);
    SetColour(glm::vec4(1, 0, 0, 1));
}

// PRIVATE
