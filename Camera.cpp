#include "Camera.h"

#include <sstream>

#include "DebugBox.h"
#include "Keyboard.h"
#include "Ray.h"

Camera::Camera(const int width, const int height, const glm::vec3& position, const glm::mat4& rotation, const float fov)
    :SphereObject(position, rotation, 2.0f, false), Width(width), Height(height)
{
    SetColour(glm::vec4(1, 1, 1, 1));
    FOV.x = fov;
    FOV.y = ((float)Height / (float)Width) * FOV.x;
}

void Camera::Update(const std::vector<WorldObject*>& worldObjects, const float deltaTime)
{
    prevPos = GetPosition();
    glm::vec3 newPos = prevPos;

    // Movement
    float positionChange = TRANS_UNITS_PER_SEC * deltaTime;

    if(Keyboard::IsKeyDown('A'))
    {
        newPos += glm::vec3(xRotMat * glm::vec4(-positionChange, 0, 0, 1.0f));
    }

    if(Keyboard::IsKeyDown('D'))
    {
        newPos += glm::vec3(xRotMat * glm::vec4(positionChange, 0, 0, 1.0f));
    }
    if(Keyboard::IsKeyDown('W'))
    {
        newPos += glm::vec3(xRotMat * glm::vec4(0, 0, -positionChange, 1.0f));
    }

    if(Keyboard::IsKeyDown('S'))
    {
        newPos += glm::vec3(xRotMat * glm::vec4(0, 0, positionChange, 1.0f));
    }

    SetPosition(newPos);

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

    // WorldObject Intersection
    if(GetPosition() != prevPos)
    {
        glm::vec3 heading = GetPosition() - prevPos;
        Ray ray(prevPos, glm::normalize(heading), 0.0f, heading.length(), NULL);
        IsectData* isectData = new IsectData();
        int nearestObjectIdx = -1;
        int nearestPortalIdx = -1;

        for(uint16_t i = 0; i < worldObjects.size(); i++)
        {
            if(worldObjects[i] == this)
            {
                continue;
            }

            if(worldObjects[i]->GetExitPortal() == NULL) // Not a portal
            {
                /*
                if(worldObjects[i]->Intersects(ray, isectData))
                {
                    nearestObjectIdx = i;
                }
                */
            }
            else
            {
                if(worldObjects[i]->GetGeometry()->Intersects(ray, isectData, this, true))
                {
                    nearestPortalIdx = i;
                }
            }
        }

        if(nearestObjectIdx != -1)
        {
            /*
            glm::vec3 heading = isectData->Entry - prevPos;
            if(heading.length() <= m_radius)
            {
                glm::vec3 inverseTravel = ray.Direction * (heading.length() - m_radius);
                SetPosition(m_position + inverseTravel);
            }
            */
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
            glm::vec3 distanceThroughPortal = isectData->Entry - GetPosition();
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
