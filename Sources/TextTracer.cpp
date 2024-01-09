#include "TextTracer.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "WorldObject.h"
#include "KdTree.h"
#include "TestScene.h"
#include "GLFWFramebuffer.h"

// Scenes
TestScene* testScene;

// World Objects
std::vector<Scene*> world;
std::vector<WorldObject*> worldObjects;
//KdTree* worldTree;

TextTracer::TextTracer()
{
    // Instantiate Main Objects
    m_framebuffer = new GLFWFramebuffer(WIDTH, HEIGHT);
    m_camera = new Camera(m_framebuffer->Width(), m_framebuffer->Height(), glm::vec3(0, 0, 0), glm::mat4(1.0), FOV);
    m_raytracer = new Raytracer(m_camera, m_framebuffer);

    // Add scenes to world vector
    testScene = new TestScene();
    world.push_back(testScene);

    worldObjects.clear();
    worldObjects.push_back(m_camera);
    for(uint16_t i = 0; i < world.size(); i++)
    {
        const std::vector<WorldObject*>* staticObjects = world[i]->GetStaticObjects();
        const std::vector<WorldObject*>* dynamicObjects = world[i]->GetDynamicObjects();
        worldObjects.insert(worldObjects.end(), staticObjects->begin(), staticObjects->end());
        worldObjects.insert(worldObjects.end(), dynamicObjects->begin(), dynamicObjects->end());
    }
}

TextTracer::~TextTracer()
{
    for(uint16_t i = 0; i < world.size(); i++)
    {
        delete world[i];
    }
    //delete worldTree;

    delete m_framebuffer;
    delete m_camera;
    delete m_raytracer;
}

void TextTracer::Update(const int worldClock)
{
    // Calculate delta time
    int elapsedTime = worldClock - m_prevWorldClock;
    m_prevWorldClock = worldClock;
    // m_deltaTime is clamped to MIN_TIMESTEP to prevent instability at large timesteps
    m_deltaTime = std::min(MIN_TIMESTEP, (float)elapsedTime / (float)CLOCKS_PER_SEC);

    // Update world and aggregate into worldObjects
    for(uint16_t i = 0; i < world.size(); i++)
    {
        world[i]->Update(m_deltaTime);
    }

    // Controls
    m_camera->Update(m_framebuffer, worldObjects, m_deltaTime);

    // Day/Night Cycle
    float lerpFactor = (-testScene->SunNormal.y + 1.0f) / 2.0f;
    m_raytracer->SkyColour = glm::mix(m_dayColour, m_nightColour, lerpFactor);
    m_raytracer->SkyLightDirection = testScene->SunNormal;

    // DEBUG: kD Tree nearest neighbour
    /*
    KdTree* tree = world[0]->GetStaticTree();
    KdNode* nearestNode = tree->NearestNeighbour(m_camera->GetPosition(), tree->GetRootNode());
    glm::vec3 pos = nearestNode->Object->GetPosition();
    std::stringstream msg;
    msg << "Nearest: X: " << pos.x << " Y: " << pos.y << " Z: " << pos.z;
    DebugBox::WriteMessage(msg, 1);
    */
}

void TextTracer::Draw()
{
    m_framebuffer->Clear();
    m_raytracer->Trace(worldObjects);
    m_framebuffer->Draw();

    // Basic FPS Counter
    static int frames = 0;
    static int frameTime = 0;
    int elapsedTime = m_deltaTime * CLOCKS_PER_SEC;
    frames++;
    frameTime += elapsedTime;
    if(frameTime > CLOCKS_PER_SEC)
    {
        DebugBox::GetInstance().Message << "FPS: " << frames << "\t";
        DebugBox::GetInstance().WriteMessage(0, 0);
        frames = 0;
        frameTime = 0;
    }
}

bool TextTracer::IsKeyDown(const char key)
{
    return m_framebuffer->IsKeyDown(key);
}
