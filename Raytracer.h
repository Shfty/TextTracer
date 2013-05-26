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

    float AmbientIntensity = 0.5f;
    glm::vec4 AmbientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    glm::vec4 SkyLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    glm::vec3 SkyLightDirection = glm::vec3(0, 1, 0);

    Raytracer(const Camera* camera, const Framebuffer* framebuffer);
    ~Raytracer();

    void Trace(const std::vector<WorldObject*>& worldObjects);

private:
    int m_width;
    int m_height;
    Camera* m_camera;
    Framebuffer* m_framebuffer;

    bool traceViewRay(Ray& ray, const std::vector<WorldObject*>& worldObjects, const int maxRecursion);
    bool traceShadowRay(Ray& ray, const std::vector<WorldObject*>& worldObjects);
    bool traceHitRay(Ray& ray, const std::vector<WorldObject*>& worldObjects);
};

#endif // RAYTRACER_H
