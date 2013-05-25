#include "TextTracer.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "WorldObject.h"
#include "KdTree.h"
#include "TestScene.h"

#ifdef LOW_RES
const int WIDTH = 32;
const int HEIGHT = 24;
#else
const int WIDTH = 64;
const int HEIGHT = 48;
#endif // LOW_RES
const float FOV = 3.141f / 4; // 45 Degrees
const int HUD_HEIGHT = 5;

// Scenes
TestScene* testScene;

// World Objects
std::vector<Scene*> world;
std::vector<WorldObject*> worldObjects;
//KdTree* worldTree;

TextTracer::TextTracer()
{
    // Setup console window size
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, WIDTH, HEIGHT + HUD_HEIGHT};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleTitle("Text Tracer");
#endif // _WIN32

    // Instantiate Main Objects
    m_framebuffer = new ConsoleFramebuffer(WIDTH, HEIGHT);
    m_camera = new Camera(m_framebuffer->Width(), m_framebuffer->Height(), glm::vec3(0, 0, 0), glm::mat4(), FOV);
    m_raytracer = new Raytracer(m_camera, m_framebuffer);

    // Add scenes to world vector
    testScene = new TestScene();
    world.push_back(testScene);
}

TextTracer::~TextTracer()
{
    for(int i = 0; i < world.size(); i++)
    {
        delete world[i];
    }
    //delete worldTree;

    delete m_framebuffer;
    delete m_camera;
    delete m_raytracer;
}

void TextTracer::Update(int worldClock)
{
    // Calculate elapsed time
    int elapsedTime = worldClock - m_prevWorldClock;
    m_prevWorldClock = worldClock;

    // Update world and aggregate into worldObjects
    worldObjects.clear();
    for(int i = 0; i < world.size(); i++)
    {
        world[i]->Update(elapsedTime);

        const std::vector<WorldObject*>* staticObjects = world[i]->GetStaticObjects();
        const std::vector<WorldObject*>* dynamicObjects = world[i]->GetDynamicObjects();
        worldObjects.insert(worldObjects.end(), staticObjects->begin(), staticObjects->end());
        worldObjects.insert(worldObjects.end(), dynamicObjects->begin(), dynamicObjects->end());
    }

    // Controls
    m_camera->Update(worldObjects, elapsedTime);

    // Day/Night Cycle
    float lerpFactor = (-testScene->SunNormal.y + 1.0f) / 2.0f;
    m_skyColour = glm::lerp(m_dayColour, m_nightColour, lerpFactor);
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
    m_framebuffer->Clear(m_skyColour);
    m_raytracer->Trace(worldObjects);
    m_framebuffer->Draw();
}
