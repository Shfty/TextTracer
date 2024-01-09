#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class Ray;
class IsectData;
class Camera;

class AABB
{
public:
    AABB() {}
    AABB(const glm::vec3& minPoint, const glm::vec3& maxPoint);
    AABB(const glm::vec3& position, const glm::vec3& minPoint, const glm::vec3& maxPoint);

    glm::vec3 GetPosition() const { return m_position; }
    glm::vec3 GetMin() const { return m_minBoundLocal; }
    glm::vec3 GetMax() const { return m_maxBoundLocal; }
    bool GetBackface() const { return m_backface; }

    void SetPosition(const glm::vec3& position);
    void SetMin(const glm::vec3& minBound);
    void SetMax(const glm::vec3& maxBound);
    void SetBackface(const bool backface) { m_backface = backface; }

    bool Contains(const glm::vec3& point);
    bool Intersects(const Ray& ray, IsectData* isectData, const Camera* camera);

private:
    void calculateWorldBounds();
    glm::vec3 cardinalDirection(const glm::vec3& inVector);

    glm::vec3 m_position;
    glm::vec3 m_minBoundLocal;
    glm::vec3 m_maxBoundLocal;
    glm::vec3 m_minBoundWorld;
    glm::vec3 m_maxBoundWorld;

    bool m_backface = false;
};

#endif // AABB_H
