#include "TextTracer.h"

#include <cmath>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "WorldObject.h"
#include "KdTree.h"
#include "TestScene.h"

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
    SHORT width = WIDTH;
    SHORT height = HEIGHT + HUD_HEIGHT;
    SMALL_RECT windowSize = {0, 0, width, height};
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

    worldObjects.clear();
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
    // Calculate elapsed time
    m_worldClock = worldClock;
    int elapsedTime = m_worldClock - m_prevWorldClock;
    m_prevWorldClock = m_worldClock;

    // Update world and aggregate into worldObjects
    for(uint16_t i = 0; i < world.size(); i++)
    {
        world[i]->Update(elapsedTime);
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

    // Basic FPS Counter
    static int prevDrawClock = 0;
    int elapsedTime = m_worldClock - prevDrawClock;
    static int frames = 0;
    static int frameTime = 0;
    frames++;
    frameTime += elapsedTime;
    if(frameTime > CLOCKS_PER_SEC)
    {
        std::stringstream msg;
        msg << "FPS: " << frames << "\t";
        DebugBox::WriteMessage(msg, TextTracer::HEIGHT, 0);
        frames = 0;
        frameTime = 0;
    }

    prevDrawClock = m_worldClock;
}
