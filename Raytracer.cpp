#include <cmath>
#include <iostream>
#include <algorithm>
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
                    m_framebuffer->PaintCell(x + cx, y + cy, traceViewRay(ray, worldObjects, MAX_BOUNCES));
                }
            }
        }
    }
}

glm::vec4 Raytracer::traceViewRay(Ray& ray, const std::vector<WorldObject*>& worldObjects, const int maxRecursion)
{
    std::vector<IsectData*> intersections;

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

        IsectData* isectData = new IsectData();
        if(worldObjects[i]->Intersects(ray, isectData))
        {
            intersections.push_back(isectData);
        }
        else
        {
            delete isectData;
        }
    }

    if(intersections.size() > 0)
    {
        // Check for portal intersections, re-cast ray to determine their final colour
        for(uint16_t i = 0; i < intersections.size(); i++)
        {
            if(intersections[i]->Object->GetExitPortal() != NULL)
            {
                if(maxRecursion > 0)
                {
                    intersections[i]->Colour = portalViewRay(ray, intersections[i], worldObjects, maxRecursion - 1);
                }
                else
                {
                    intersections[i]->Colour = glm::vec4(0, 0, 0, 1);
                }
            }
        }

        // Sort intersections from near-far
        std::sort(intersections.begin(), intersections.end(), distanceSortPredicate());

        // Iterate through sorted intersections and sum colour
        //  Break out of loop if colour alpha >= 1.0f
        glm::vec4 finalColour = glm::vec4(0);
        float alpha = 0.0f;
        for(uint16_t i = 0; i < intersections.size(); i++)
        {
            alpha += intersections[i]->Colour.a;

            glm::vec4 objectColour = intersections[i]->Colour;
            float objectAlpha = intersections[i]->Colour.a;

#ifndef DISABLE_LIGHTING
            if(!intersections[i]->Object->Fullbright)
            {
                Ray shadowRay(intersections[i]->Entry, -glm::normalize(SkyLightDirection), NEAR_PLANE, FAR_PLANE, intersections[i]->Object);
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

                objectColour *= SkyLightColour;
                objectColour *= AmbientLightColour;
                objectColour *= brightness;
            }
#endif // DISABLE_LIGHTING

            finalColour += objectColour * objectAlpha;

            if(alpha >= 1.0f)
            {
                break;
            }
        }

        if(alpha < 1.0f)
        {
            finalColour += SkyColour * (1.0f - alpha);
        }

        for(int i = 0; i < 4; i++)
        {
            finalColour[i] = glm::min(finalColour[i], 1.0f);
        }

        return finalColour;
    }

    return SkyColour;
}

glm::vec4 Raytracer::portalViewRay(Ray& ray, IsectData* isectData, const std::vector<WorldObject*>& worldObjects, const int maxRecursion)
{
    // Compute angular difference between portals
    glm::vec3 backward = glm::vec3(0, 0, 1);
    glm::vec3 entryPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * isectData->Object->GetRotation()));
    glm::vec3 exitPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * -isectData->Object->GetExitPortal()->GetRotation()));
    float angle = glm::acos(glm::dot(entryPortalNormal, exitPortalNormal));
    glm::vec3 axis = glm::normalize(glm::cross(entryPortalNormal, exitPortalNormal));

    // Compute new origin
    glm::vec3 inOriginRelative = isectData->Entry - isectData->Object->GetPosition();
    glm::vec3 inOriginRelativeRotated =
        glm::vec3(
            glm::rotate(glm::degrees(angle), axis) *
            glm::vec4(inOriginRelative, 1.0f)
        );
    glm::vec3 rayOrigin = inOriginRelativeRotated + isectData->Object->GetExitPortal()->GetPosition();

    glm::mat4 rayOrientation = glm::rotate(glm::degrees(angle), axis);
    glm::vec3 rayDirection = glm::vec3(rayOrientation * glm::vec4(ray.Direction, 1.0f));

    ray.Origin = rayOrigin;
    ray.Direction = rayDirection;
    ray.ParentObject = isectData->Object->GetExitPortal();

    return traceViewRay(ray, worldObjects, maxRecursion);
}

bool Raytracer::traceShadowRay(Ray& ray, const std::vector<WorldObject*>& worldObjects)
{
    for(uint16_t i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == ray.ParentObject) continue;

        if(!worldObjects[i]->CastShadow) continue;

        if(worldObjects[i]->Intersects(ray, (IsectData*)NULL))
        {
            return true;
        }
    }

    return false;
}

bool Raytracer::traceHitRay(Ray& ray, const std::vector<WorldObject*>& worldObjects)
{
    for(uint16_t i = 0; i < worldObjects.size(); i++)
    {
        if(worldObjects[i] == ray.ParentObject) continue;

        if(worldObjects[i]->Intersects(ray, (IsectData*)NULL))
        {
            return true;
        }
    }

    return false;
}
