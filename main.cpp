#define REALTIME
//#undef REALTIME
#define LOW_RES
//#undef LOW_RES

#include <cmath>
#include <ctime>
#include <vector>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include "SphereObject.h"
#include "PlaneObject.h"
#include "PolyObject.h"
#include "ConsoleFramebuffer.h"
#include "Raytracer.h"
#include "Camera.h"
#include "Keyboard.h"

#ifdef LOW_RES
const int WIDTH = 32;
const int HEIGHT = 32;
const int RENDER_FPS = 30;
#else
const int WIDTH = 64;
const int HEIGHT = 64;
const int RENDER_FPS = 15;
#endif
const int HUD_HEIGHT = 5;
const float FOV = 3.141f / 4; // 45 Degrees
const int UPDATE_FPS = 60;

ConsoleFramebuffer* framebuffer;
Camera* camera;
Raytracer* raytracer;

int worldClock = 0;
int prevRenderTicks = 0;
int prevUpdateTicks = 0;
bool redraw = true;
bool update = true;

float cameraAngleX = 0;
float cameraAngleY = 0;

bool quit = false;

std::vector<WorldObject*> worldObjects;
SphereObject* floatSphere;

void Init()
{
    // Setup console window size
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT windowSize = {0, 0, WIDTH, HEIGHT + HUD_HEIGHT};
    SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    SetConsoleTitle("Text Tracer");
#endif // _WIN32

    //Instantiate Framebuffer
    framebuffer = new ConsoleFramebuffer(WIDTH, HEIGHT);
    camera = new Camera(framebuffer->Width(), framebuffer->Height(), glm::vec3(0, 0, 0), glm::mat4(), FOV);
    raytracer = new Raytracer(camera, framebuffer);

    // Instantiate world objects
    // Floor
    PlaneObject* testFloor = new PlaneObject(glm::vec3(0, 5, 0), glm::rotate(90.0f, glm::vec3(1, 0, 0)), false);
    testFloor->ObjectColour = glm::vec4(0.5, 0.5, 0.5, 1);


    // Walls
    PlaneObject* testWallN = new PlaneObject(glm::vec3(0, 0, 40), glm::rotate(180.0f, glm::vec3(0, 1, 0)), true);
    testWallN->ObjectColour = glm::vec4(0, 1, 1, 1);
    PlaneObject* testWallE = new PlaneObject(glm::vec3(40, 0, 0), glm::rotate(-90.0f, glm::vec3(0, 1, 0)), true);
    testWallE->ObjectColour = glm::vec4(1, 1, 0, 1);
    PlaneObject* testWallS = new PlaneObject(glm::vec3(0, 0, -80), glm::rotate(0.0f, glm::vec3(0, 1, 0)), true);
    testWallS->ObjectColour = glm::vec4(1, 1, 1, 1);
    PlaneObject* testWallW = new PlaneObject(glm::vec3(-40, 0, 0), glm::rotate(90.0f, glm::vec3(0, 1, 0)), true);
    testWallW->ObjectColour = glm::vec4(1, 0, 1, 1);

    // Poly Portal
    PolyObject* polyPortalIn = new PolyObject(glm::vec3(-10, 0, -50), glm::rotate(-90.0f, glm::vec3(0, 1, 0)), 4.0f, 5, true);
    PolyObject* polyPortalInOutline = new PolyObject(glm::vec3(-10, 0, -50), glm::rotate(-90.0f, glm::vec3(0, 1, 0)), 5.0f, 5, true);
    PolyObject* polyPortalOut = new PolyObject(glm::vec3(10, 0, -50), glm::rotate(90.0f, glm::vec3(0, 1, 0)), 4.0f, 5, true);
    PolyObject* polyPortalOutOutline = new PolyObject(glm::vec3(10, 0, -50), glm::rotate(90.0f, glm::vec3(0, 1, 0)), 5.0f, 5, true);

    polyPortalIn->ObjectColour = glm::vec4(0, 0, 0, 1);
    polyPortalIn->Portal = true;
    polyPortalIn->ExitPortal = polyPortalOut;
    polyPortalIn->ExitDecoration = polyPortalInOutline;

    polyPortalOut->ObjectColour = glm::vec4(0, 0, 0, 1);
    polyPortalOut->Portal = true;
    polyPortalOut->ExitPortal = polyPortalIn;
    polyPortalOut->ExitDecoration = polyPortalOutOutline;

    // Sphere Portal
    SphereObject* spherePortalIn = new SphereObject(glm::vec3(20, 0, -20), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 9.0f, false);
    SphereObject* spherePortalInOutline = new SphereObject(glm::vec3(20, 0, -20), glm::mat4(), 10.0f, true);
    SphereObject* spherePortalOut = new SphereObject(glm::vec3(-20, 0, -20), glm::rotate(180.0f, glm::vec3(0, 1, 0)), 9.0f, false);
    SphereObject* spherePortalOutOutline = new SphereObject(glm::vec3(-20, 0, -20), glm::mat4(), 10.0f, true);

    spherePortalIn->ObjectColour = glm::vec4(0, 0, 0, 1);
    spherePortalIn->Portal = true;
    spherePortalIn->ExitPortal = spherePortalOut;
    spherePortalIn->ExitDecoration = spherePortalInOutline;

    spherePortalOut->ObjectColour = glm::vec4(0, 0, 0, 1);
    spherePortalOut->Portal = true;
    spherePortalOut->ExitPortal = spherePortalIn;
    spherePortalOut->ExitDecoration = spherePortalOutOutline;

    // Triangle
    PolyObject* testTri = new PolyObject(glm::vec3(-20, 0, 20), glm::rotate(135.0f, glm::vec3(0, 1, 0)), 7.5f, 3, true);
    testTri->ObjectColour = glm::vec4(0, 0, 1, 1);

    // Floating Pentagon
    PolyObject* testPent = new PolyObject(glm::vec3(0, -2.5, -12.5), glm::rotate(-90.0f, glm::vec3(1, 0, 0)), 5.0f, 5, true);
    testPent->ObjectColour = glm::vec4(1, 1, 0, 1);

    // Moving Sphere
    floatSphere = new SphereObject(glm::vec3(20, 0, 20), glm::mat4(), 2.0f, false);
    floatSphere->ObjectColour = glm::vec4(0, 1, 0, 1);

    // Add camera to worldObjects
    worldObjects.push_back(camera);

    // Add other objects to worldObjects
    worldObjects.push_back(floatSphere);

    worldObjects.push_back(testFloor);

    worldObjects.push_back(testWallN);
    worldObjects.push_back(testWallE);
    worldObjects.push_back(testWallS);
    worldObjects.push_back(testWallW);

    worldObjects.push_back(testTri);

    worldObjects.push_back(polyPortalIn);
    worldObjects.push_back(polyPortalInOutline);
    worldObjects.push_back(polyPortalOut);
    worldObjects.push_back(polyPortalOutOutline);

    worldObjects.push_back(spherePortalIn);
    worldObjects.push_back(spherePortalInOutline);
    worldObjects.push_back(spherePortalOut);
    worldObjects.push_back(spherePortalOutOutline);

    worldObjects.push_back(testPent);
}

void Update()
{
    // Controls
    camera->Update(worldObjects);

    // Quit
    if(Keyboard::IsKeyDown(0x1B)) // Esc
    {
        quit = true;
    }

    // Object movement
    glm::vec3 spherePosition = floatSphere->GetPosition();
    spherePosition.y = -10 + sin(worldClock / (float)CLOCKS_PER_SEC) * 10.0f;
    floatSphere->SetPosition(spherePosition);
}

void Draw()
{
    framebuffer->Clear(glm::vec4(0, 1, 1, 1));
    raytracer->Trace(worldObjects);
    framebuffer->Draw();
}

void Cleanup()
{
    for(int i = 0; i < worldObjects.size(); i++)
    {
        delete worldObjects[i];
    }

    delete framebuffer;
    delete camera;
    delete raytracer;
}

int main()
{
    // Initialization
    Init();

#ifdef REALTIME
    while(!quit)
    {
#endif
        // Cap update to tick rate
        worldClock = clock();
        int updateTicks = worldClock % (CLOCKS_PER_SEC / UPDATE_FPS);
        if(updateTicks == 0 && prevUpdateTicks != 0)
        {
            update = true;
        }
        prevUpdateTicks = updateTicks;

        if(update)
        {
            Update();
            update = false;
        }

        // Cap rendering to frame rate
        int renderTicks = worldClock % (CLOCKS_PER_SEC / RENDER_FPS);
        if(renderTicks == 0 && prevRenderTicks != 0)
        {
            redraw = true;
        }
        prevRenderTicks = renderTicks;

        if(redraw)
        {
            Draw();
            redraw = false;
        }
#ifdef REALTIME
    }
#endif

    Cleanup();
    return 0;
}
