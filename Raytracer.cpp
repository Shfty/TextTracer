#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#include "Raytracer.h"
#include "Ray.h"

Raytracer::Raytracer(const Camera* camera, const Framebuffer* framebuffer)
    :m_camera((Camera*)camera),
    m_framebuffer((Framebuffer*)framebuffer)
{
}

Raytracer::~Raytracer()
{
}

void Raytracer::Trace(const std::vector<WorldObject*>& worldObjects)
{
    for(int x = 0; x < m_camera->Width; x += CELL_SIZE_X)
    {
        for(int y = 0; y < m_camera->Height; y += CELL_SIZE_Y)
        {
            for(int cx = 0; cx < CELL_SIZE_X; cx++)
            {
                for(int cy = 0; cy < CELL_SIZE_Y; cy++)
                {
                    float xMag = ((((float)(x + cx) * 2.0f) - (float)m_camera->Width) / (float)m_camera->Width) * tan(m_camera->FOV.x);
                    float yMag = ((((float)(y + cy) * 2.0f) - (float)m_camera->Height) / (float)m_camera->Height) * tan(m_camera->FOV.y);
                    glm::vec3 direction = glm::vec3(m_camera->GetRotation() * glm::vec4(glm::normalize(glm::vec3(xMag, yMag, -1.0f)), 0.0f));

                    Ray ray(m_camera->GetPosition(), direction, NEAR_PLANE, FAR_PLANE, m_camera);
                    if(traceViewRay(ray, worldObjects, 5))
                    {
                        m_framebuffer->PaintCell(x + cx, y + cy, ray.Colour);
                    }
                }
            }
        }
    }
}

bool Raytracer::traceViewRay(Ray& ray, const std::vector<WorldObject*>& worldObjects, const int maxRecursion)
{
    int nearestObjectIdx = -1;
    IsectData isectData;

    for(uint16_t i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == ray.ParentObject)
        {
            continue;
        }

        if(ray.ParentObject != NULL)
        {
            std::vector<WorldObject*> rayParentChildren = ray.ParentObject->GetChildren();
            for(uint16_t o = 0; o < rayParentChildren.size(); o++)
            {
                if(worldObjects[i] == rayParentChildren[o])
                {
                    continue;
                }
            }
        }

        if(worldObjects[i]->Intersects(ray, isectData))
        {
            nearestObjectIdx = i;
        }
    }

    if(nearestObjectIdx != -1)
    {
        if(worldObjects[nearestObjectIdx]->GetExitPortal() != NULL && maxRecursion > 0)
        {
            // Compute angular difference between portals
            glm::vec3 backward = glm::vec3(0, 0, 1);
            glm::vec3 entryPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestObjectIdx]->GetRotation()));
            glm::vec3 exitPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * -worldObjects[nearestObjectIdx]->GetExitPortal()->GetRotation()));
            float angle = glm::acos(glm::dot(entryPortalNormal, exitPortalNormal));
            glm::vec3 axis = glm::normalize(glm::cross(entryPortalNormal, exitPortalNormal));

            // Compute new origin
            glm::vec3 inOriginRelative = isectData.Entry - worldObjects[nearestObjectIdx]->GetPosition();
            glm::vec3 inOriginRelativeRotated =
                glm::vec3(
                    glm::rotate(glm::degrees(angle), axis) *
                    glm::vec4(inOriginRelative, 1.0f)
                );
            glm::vec3 rayOrigin = inOriginRelativeRotated + worldObjects[nearestObjectIdx]->GetExitPortal()->GetPosition();

            glm::mat4 rayOrientation = glm::rotate(glm::degrees(angle), axis);
            glm::vec3 rayDirection = glm::vec3(rayOrientation * glm::vec4(ray.Direction, 1.0f));

            ray.Origin = rayOrigin;
            ray.Direction = rayDirection;
            ray.ParentObject = worldObjects[nearestObjectIdx]->GetExitPortal();
            ray.FarPlane = FAR_PLANE;
            return traceViewRay(ray, worldObjects, maxRecursion - 1);
        }
        else
        {
            ray.Colour = worldObjects[nearestObjectIdx]->ObjectColour;
#ifndef DISABLE_LIGHTING
            if(!worldObjects[nearestObjectIdx]->Fullbright)
            {
                Ray shadowRay(isectData.Entry, -glm::normalize(SkyLightDirection), NEAR_PLANE, FAR_PLANE, worldObjects[nearestObjectIdx]);
                bool occluded = traceShadowRay(shadowRay, worldObjects);
                float diffuseFactor = glm::max(0.0f, glm::dot(ray.Direction, glm::normalize(SkyLightDirection)));

                float brightness;
                if(occluded)
                {
                    brightness = AmbientIntensity;
                }
                else
                {
                    brightness = diffuseFactor + AmbientIntensity;
                }

                ray.Colour *= SkyLightColour;
                ray.Colour *= AmbientLightColour;
                ray.Colour *= brightness;
            }
#endif // DISABLE_LIGHTING
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Raytracer::traceShadowRay(Ray& ray, const std::vector<WorldObject*>& worldObjects)
{
    IsectData isectData;

    for(uint16_t i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == ray.ParentObject) continue;

        if(!worldObjects[i]->CastShadow) continue;

        if(worldObjects[i]->Intersects(ray, isectData))
        {
            return true;
        }
    }

    return false;
}

bool Raytracer::traceHitRay(Ray& ray, const std::vector<WorldObject*>& worldObjects)
{
    IsectData isectData;

    for(uint16_t i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == ray.ParentObject) continue;

        if(worldObjects[i]->Intersects(ray, isectData))
        {
            return true;
        }
    }

    return false;
}
