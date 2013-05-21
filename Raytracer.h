#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "WorldObject.h"
#include "Framebuffer.h"
#include "Camera.h"

class Raytracer
{
public:
    const float NEAR_PLANE = 0.0f;
    const float FAR_PLANE = 10000.0f;
    const int CELL_SIZE_X = 8;
    const int CELL_SIZE_Y = 8;

    Raytracer(Camera& camera, Framebuffer& framebuffer);
    ~Raytracer();

    void Trace(std::vector<WorldObject*> worldObjects);

private:
    int m_width;
    int m_height;
    Camera& m_camera;
    Framebuffer& m_framebuffer;

    bool traceRay(glm::vec3 origin, glm::vec3 direction, std::vector<WorldObject*> worldObjects, WorldObject* parentObject, int maxRecursion, Colour& rayColour);
};

#endif // RAYTRACER_H
