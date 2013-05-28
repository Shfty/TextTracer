#include "Camera.h"

#include <sstream>

#include "DebugBox.h"
#include "Keyboard.h"
#include "Ray.h"

Camera::Camera(const int width, const int height, const glm::vec3& position, const glm::mat4& rotation, const float fov)
    :SphereObject(position, rotation, 1.0f, false), Width(width), Height(height)
{
    ObjectColour = glm::vec4(1, 1, 1, 1);
    FOV.x = fov;
    FOV.y = ((float)Height / (float)Width) * FOV.x;
}

void Camera::Update(const std::vector<WorldObject*>& worldObjects, const float deltaTime)
{
    prevPos = m_position;

    // Movement
    float positionChange = TRANS_UNITS_PER_SEC * deltaTime;

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
    float rotationChange = ROT_UNITS_PER_SEC * deltaTime;

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
        Ray ray(prevPos, glm::normalize(heading), 0.0f, heading.length(), NULL);
        IsectData* isectData = new IsectData();
        int nearestPortalIdx = -1;

        for(uint16_t i = 0; i < worldObjects.size(); i++)
        {
            if(worldObjects[i]->GetExitPortal() == NULL) // Not a portal
            {
                continue;
            }
            else
            {
                if(worldObjects[i]->IntersectsPortal(ray, isectData, m_rotation))
                {
                    nearestPortalIdx = i;
                }
            }
        }

        if(nearestPortalIdx != -1)
        {
            // Compute angular difference between portals
            glm::vec3 backward = glm::vec3(0, 0, 1);
            glm::vec3 entryPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * worldObjects[nearestPortalIdx]->GetRotation()));
            glm::vec3 exitPortalNormal = glm::normalize(glm::vec3(glm::vec4(backward, 1.0f) * -worldObjects[nearestPortalIdx]->GetExitPortal()->GetRotation()));
            float angle = glm::acos(glm::dot(entryPortalNormal, exitPortalNormal));
            glm::vec3 axis = glm::normalize(glm::cross(entryPortalNormal, exitPortalNormal));

            // Compute new rotation
            glm::mat4 newRotation = glm::rotate(glm::degrees(angle), axis);

            // Compute exit point
            glm::vec3 entryPointLocal = isectData->Entry - worldObjects[nearestPortalIdx]->GetPosition();
            glm::vec3 entryPointLocalRotated =
            glm::vec3(
                glm::rotate(glm::degrees(angle), axis) *
                glm::vec4(entryPointLocal, 1.0f)
            );
            glm::vec3 exitPoint = entryPointLocalRotated + worldObjects[nearestPortalIdx]->GetExitPortal()->GetPosition();

            // Compute travel distance
            glm::vec3 distanceThroughPortal = isectData->Entry - m_position;
            glm::vec3 distanceThroughPortalRotated = glm::vec3(newRotation * glm::vec4(distanceThroughPortal, 1.0f));

            DebugBox::GetInstance().Message << "Distance Through: X: " << distanceThroughPortalRotated.x << "Y: " << distanceThroughPortalRotated.y << "Z: " << distanceThroughPortalRotated.z;
            DebugBox::GetInstance().WriteMessage(24, 2);

            glm::vec3 outPosition = exitPoint + distanceThroughPortalRotated;

            SetPosition(outPosition);
            xRotMat *= newRotation;
        }

        delete isectData;
    }

    SetRotation(xRotMat * yRotMat);
}
