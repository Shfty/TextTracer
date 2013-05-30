#include "AABBObject.h"
#include "AABBGeometry.h"

// PUBLIC
AABBObject::AABBObject(const glm::vec3& position, const glm::mat4& rotation, const AABB& bounds)
{
    m_geometry = new AABBGeometry();
    SetPosition(position);
    SetBounds(bounds);
    SetColour(glm::vec4(1, 1, 1, 1));
}
