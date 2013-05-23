#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#include "Raytracer.h"
#include "Ray.h"

glm::vec4 ambientLightColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
glm::vec3 ambientLightDirection = glm::vec3(0, 1, 0);

Raytracer::Raytracer(Camera* camera, Framebuffer* framebuffer)
    :m_camera(camera),
    m_framebuffer(framebuffer)
{
}

Raytracer::~Raytracer()
{
}

void Raytracer::Trace(std::vector<WorldObject*> worldObjects)
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

                    glm::vec4 rayColour;
                    if(traceViewRay(m_camera->GetPosition(), direction, worldObjects, m_camera, 5, rayColour))
                    {
                        m_framebuffer->PaintCell(x + cx, y + cy, rayColour);
                    }
                }
            }
        }
    }
}

// parentObject = object that the ray originates from
bool Raytracer::traceViewRay(glm::vec3 origin, glm::vec3 direction, std::vector<WorldObject*> worldObjects, WorldObject* parentObject, int maxRecursion, glm::vec4& rayColour)
{
    Ray ray(origin, direction, NEAR_PLANE, FAR_PLANE);
    int nearestObjectIdx = -1;
    IsectData isectData;

    for(int i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == parentObject)
        {
            continue;
        }

        if(parentObject != NULL && worldObjects[i] == parentObject->ExitDecoration)
        {
            continue;
        }

        if(worldObjects[i]->Intersects(ray, isectData))
        {
            nearestObjectIdx = i;
        }
    }

    if(nearestObjectIdx != -1)
    {
        if(worldObjects[nearestObjectIdx]->Portal && maxRecursion > 0)
        {
            // Compute new origin
            glm::vec3 inIntersectionRelative = isectData.Entry - worldObjects[nearestObjectIdx]->GetPosition();
            glm::vec3 inIntersectionRelativeRotated =
                glm::vec3(
                    glm::rotate(180.0f, glm::vec3(0, 1, 0))*
                    glm::vec4(inIntersectionRelative, 1.0f)
                );
            glm::vec3 rayOrigin = inIntersectionRelativeRotated + worldObjects[nearestObjectIdx]->ExitPortal->GetPosition();

            // Compute angular difference between portals
            glm::vec3 backward = glm::vec3(0, 0, 1);
            glm::vec3 entryPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestObjectIdx]->GetRotation()));
            glm::vec3 exitPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestObjectIdx]->ExitPortal->GetRotation()));
            float angle = glm::acos(glm::dot(entryPortalNormal, exitPortalNormal));
            glm::vec3 axis = glm::normalize(glm::cross(entryPortalNormal, exitPortalNormal));
            glm::mat4 rayOrientation = glm::rotate(180.0f, axis) * glm::rotate(glm::degrees(angle), axis);

            return traceViewRay(
                            rayOrigin,
                            glm::vec3(rayOrientation * glm::vec4(direction, 1.0f)),
                            worldObjects,
                            worldObjects[nearestObjectIdx]->ExitPortal,
                            maxRecursion - 1,
                            rayColour
                            );
        }
        else
        {
            glm::vec4 objectColour = worldObjects[nearestObjectIdx]->ObjectColour;
            bool occluded = traceHitRay(isectData.Entry, -ambientLightDirection, worldObjects, worldObjects[nearestObjectIdx]);


            if(occluded)
            {
                rayColour = objectColour * 0.5f;
            }
            else
            {
                rayColour = objectColour * ambientLightColour;
            }
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Raytracer::traceHitRay(glm::vec3 origin, glm::vec3 direction, std::vector<WorldObject*> worldObjects, WorldObject* parentObject)
{
    Ray ray(origin, direction, NEAR_PLANE, FAR_PLANE);
    IsectData isectData;

    for(int i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == parentObject) continue;

        if(worldObjects[i]->Intersects(ray, isectData))
        {
            return true;
        }
    }
}
