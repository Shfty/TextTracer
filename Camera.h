#ifndef CAMERA_H
#define CAMERA_H

#include <vector>

#include "SphereObject.h"

const float TRANS_UNITS_PER_SEC = 20.0f;
const float ROT_UNITS_PER_SEC = 135.0f;

struct Camera : SphereObject
{
public:
    Camera(const int width, const int height, const glm::vec3& position, const glm::mat4& rotation, const float fov);

    void Update(const std::vector<WorldObject*>& worldObjects, const float deltaTime);

    int Width;
    int Height;
    glm::vec2 FOV;

private:
    glm::vec2 lookAngle = glm::vec2(0, 0);
    glm::mat4 xRotMat = glm::mat4();
    glm::mat4 yRotMat = glm::mat4();
    glm::vec3 prevPos = glm::vec3(0, 0, 0);
};

#endif // CAMERA_H
