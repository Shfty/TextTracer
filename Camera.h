#ifndef CAMERA_H
#define CAMERA_H

#include <sstream>

#include "DebugBox.h"
#include "SphereObject.h"
#include "Keyboard.h"

const float TRANS_UNITS_PER_MS = 20.0f / 1000.0f;
const float ROT_UNITS_PER_MS = 135.0f / 1000.0f;

struct Camera : SphereObject
{
public:
    int Width;
    int Height;
    glm::vec2 FOV;

private:
    glm::vec2 lookAngle = glm::vec2(0, 0);
    glm::mat4 xRotMat = glm::mat4();
    glm::mat4 yRotMat = glm::mat4();
    glm::vec3 prevPos = glm::vec3(0, 0, 0);
    bool intersectPortal = false;

public:
    Camera(int width, int height, glm::vec3 position, glm::mat4 rotation, float fov)
        :SphereObject(position, rotation, 2.0f, false), Width(width), Height(height)
    {
        ObjectColour = glm::vec4(1, 1, 1, 1);
        FOV.x = fov;
        FOV.y = ((float)Height / (float)Width) * FOV.x;
    }

    void Update(std::vector<WorldObject*> worldObjects, int elapsedTime)
    {
        // Movement
        float positionChange = TRANS_UNITS_PER_MS * elapsedTime;

        if(Keyboard::IsKeyDown('A'))
        {
            m_position += glm::vec3(xRotMat * glm::vec4(-positionChange, 0, 0, 1.0f));
        }

        if(Keyboard::IsKeyDown('D'))
        {
            m_position += glm::vec3(xRotMat * glm::vec4(positionChange, 0, 0, 1.0f));
        }
        if(Keyboard::IsKeyDown('W'))
        {
            m_position += glm::vec3(xRotMat * glm::vec4(0, 0, -positionChange, 1.0f));
        }

        if(Keyboard::IsKeyDown('S'))
        {
            m_position += glm::vec3(xRotMat * glm::vec4(0, 0, positionChange, 1.0f));
        }

        // Rotation
        float rotationChange = ROT_UNITS_PER_MS * elapsedTime;

        if(Keyboard::IsKeyDown('J'))
        {
            xRotMat *= glm::rotate(glm::mat4(), rotationChange, glm::vec3(0, 1, 0));
        }

        if(Keyboard::IsKeyDown('L'))
        {
            xRotMat *= glm::rotate(glm::mat4(), -rotationChange, glm::vec3(0, 1, 0));
        }

        if(Keyboard::IsKeyDown('I'))
        {
            yRotMat *= glm::rotate(glm::mat4(), -rotationChange, glm::vec3(1, 0, 0));
        }

        if(Keyboard::IsKeyDown('K'))
        {
            yRotMat *= glm::rotate(glm::mat4(), rotationChange, glm::vec3(1, 0, 0));
        }

        //Portal Intersection
        if(m_position != prevPos)
        {
            glm::vec3 heading = m_position - prevPos;
            Ray ray(prevPos, glm::normalize(heading), 0.0f, heading.length());
            IsectData isectData;
            int nearestPortalIdx = -1;

            for(int i = 0; i < worldObjects.size(); i++)
            {
                if(worldObjects[i] == this)
                {
                    continue;
                }

                if(worldObjects[i]->Portal)
                {
                    if(worldObjects[i]->IntersectsPortal(ray, isectData, m_rotation))
                    {
                        nearestPortalIdx = i;
                    }
                }
            }

            if(nearestPortalIdx != -1 && !intersectPortal)
            {
                // Compute new rotation
                glm::vec3 backward = glm::vec3(0, 0, 1);
                glm::vec3 entryPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestPortalIdx]->GetRotation()));
                glm::vec3 exitPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestPortalIdx]->ExitPortal->GetRotation()));
                glm::vec3 axis = glm::normalize(glm::cross(entryPortalNormal, exitPortalNormal));
                float angle = glm::acos(glm::dot(entryPortalNormal, exitPortalNormal));
                glm::mat4 newRotation = glm::rotate(180.0f, axis) * glm::rotate(glm::degrees(angle), axis);

                xRotMat *= newRotation;

                // Compute new position
                glm::vec3 inIntersectionRelative = isectData.Entry - worldObjects[nearestPortalIdx]->GetPosition();
                glm::vec3 outIntersection = inIntersectionRelative + worldObjects[nearestPortalIdx]->ExitPortal->GetPosition();
                glm::vec3 outPositionRelative = isectData.Entry - m_position;
                glm::vec3 outPositionRelativeRotated = glm::vec3(newRotation * glm::vec4(outPositionRelative, 1.0f));

                SetPosition(outIntersection + outPositionRelativeRotated);

                intersectPortal = true;
            }
            else
            {
                intersectPortal = false;
            }

            prevPos = m_position;
        }

        SetRotation(xRotMat * yRotMat);
    }

    virtual void SetPosition(glm::vec3 position) { m_position = position; }
    virtual void SetRotation(glm::mat4 rotation) { m_rotation = rotation; }
};

#endif // CAMERA_H
