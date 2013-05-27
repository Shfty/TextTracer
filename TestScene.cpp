#include "TestScene.h"

#include "PlaneObject.h"
#include "ConvexPolyObject.h"
#include "DiscObject.h"
#include "KdTree.h"

const float SUN_ROTATION_PER_SEC = 20.0f;

SphereObject* sunSphere;
SphereObject* moonSphere;

TestScene::TestScene()
{
    // STATIC OBJECTS
    // Floor
    PlaneObject* testFloor = new PlaneObject(glm::vec3(0, 5, 0), glm::rotate(90.0f, glm::vec3(1, 0, 0)), false);
    testFloor->ObjectColour = glm::vec4(0.5, 0.5, 0.5, 1);
    m_staticObjects.push_back(testFloor);

    // Walls
    ConvexPolyObject* testWallN = new ConvexPolyObject(glm::vec3(0, 50, 40), glm::rotate(180.0f, glm::vec3(0, 1, 0)), 100.0f, 4, true);
    testWallN->ObjectColour = glm::vec4(1, 0, 0, 1);
    m_staticObjects.push_back(testWallN);

    ConvexPolyObject* testWallE = new ConvexPolyObject(glm::vec3(40, 50, -20), glm::rotate(-90.0f, glm::vec3(0, 1, 0)), 100.0f, 4, true);
    testWallE->ObjectColour = glm::vec4(1, 1, 0, 1);
    m_staticObjects.push_back(testWallE);

    ConvexPolyObject* testWallS = new ConvexPolyObject(glm::vec3(0, 50, -80), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 100.0f, 4, true);
    testWallS->ObjectColour = glm::vec4(1, 1, 1, 1);
    m_staticObjects.push_back(testWallS);

    ConvexPolyObject* testWallW = new ConvexPolyObject(glm::vec3(-40, 50, -20), glm::rotate(90.0f, glm::vec3(0, 1, 0)), 100.0f, 4, true);
    testWallW->ObjectColour = glm::vec4(1, 0, 1, 1);
    m_staticObjects.push_back(testWallW);

    // Poly Portal
    ConvexPolyObject* polyPortalIn = new ConvexPolyObject(glm::vec3(-10, 0, -50), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 4.0f, 5, true);
    ConvexPolyObject* polyPortalInOutline = new ConvexPolyObject(glm::vec3(-10, 0, -50), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 5.0f, 5, true);
    ConvexPolyObject* polyPortalOut = new ConvexPolyObject(glm::vec3(10, 0, -50), glm::rotate(90.0f, glm::vec3(0, 1, 0)), 4.0f, 5, true);
    ConvexPolyObject* polyPortalOutOutline = new ConvexPolyObject(glm::vec3(10, 0, -50), glm::rotate(90.0f, glm::vec3(0, 1, 0)), 5.0f, 5, true);

    polyPortalIn->SetExitPortal(polyPortalOut);
    polyPortalIn->AddChild(polyPortalInOutline);
    polyPortalIn->ObjectColour = glm::vec4(0, 0, 0, 1);

    polyPortalOut->SetExitPortal(polyPortalIn);
    polyPortalOut->AddChild(polyPortalOutOutline);
    polyPortalOut->ObjectColour = glm::vec4(0, 0, 0, 1);

    m_staticObjects.push_back(polyPortalIn);
    m_staticObjects.push_back(polyPortalInOutline);
    m_staticObjects.push_back(polyPortalOut);
    m_staticObjects.push_back(polyPortalOutOutline);

    // Sphere Portal
    SphereObject* spherePortalIn = new SphereObject(glm::vec3(20, 0, -20), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 5.0f, true);
    SphereObject* spherePortalInOutline = new SphereObject(glm::vec3(20, 0, -20), glm::mat4(), 6.0f, true);
    SphereObject* spherePortalOut = new SphereObject(glm::vec3(-20, 0, -20), glm::rotate(180.0f, glm::vec3(0, 1, 0)), 5.0f, true);
    SphereObject* spherePortalOutOutline = new SphereObject(glm::vec3(-20, 0, -20), glm::mat4(), 6.0f, true);

    spherePortalIn->ObjectColour = glm::vec4(0, 0, 0, 1);
    spherePortalIn->SetExitPortal(spherePortalOut);
    spherePortalIn->AddChild(spherePortalInOutline);

    spherePortalOut->ObjectColour = glm::vec4(0, 0, 0, 1);
    spherePortalOut->SetExitPortal(spherePortalIn);
    spherePortalOut->AddChild(spherePortalOutOutline);

    m_staticObjects.push_back(spherePortalIn);
    m_staticObjects.push_back(spherePortalInOutline);
    m_staticObjects.push_back(spherePortalOut);
    m_staticObjects.push_back(spherePortalOutOutline);

    // Disc Portal
    DiscObject* discPortalIn = new DiscObject(glm::vec3(20, 0, -80), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 4.0f, true);
    DiscObject* discPortalInOutline = new DiscObject(glm::vec3(20, 0, -80), glm::rotate(0.0f, glm::vec3(0, 1, 0)), 5.0f, true);
    DiscObject* discPortalOut = new DiscObject(glm::vec3(-20, -15, 40), glm::rotate(180.0f, glm::vec3(0, 1, 0)), 4.0f, true);
    DiscObject* discPortalOutOutline = new DiscObject(glm::vec3(-20, -15, 40), glm::rotate(180.0f, glm::vec3(0, 1, 0)), 5.0f, true);

    discPortalIn->SetExitPortal(discPortalOut);
    discPortalIn->AddChild(discPortalInOutline);
    discPortalInOutline->ObjectColour = glm::vec4(1, 0, 0, 1);

    discPortalOut->SetExitPortal(discPortalIn);
    discPortalOut->AddChild(discPortalOutOutline);
    discPortalOutOutline->ObjectColour = glm::vec4(0, 1, 1, 1);

    m_staticObjects.push_back(discPortalIn);
    m_staticObjects.push_back(discPortalInOutline);
    m_staticObjects.push_back(discPortalOut);
    m_staticObjects.push_back(discPortalOutOutline);

    // Triangle
    ConvexPolyObject* testTri = new ConvexPolyObject(glm::vec3(-20, 0, 20), glm::rotate(5.0f, glm::vec3(1, 0, 0)), 7.5f, 3, true);
    testTri->ObjectColour = glm::vec4(0, 0, 1, 1);
    m_staticObjects.push_back(testTri);

    // Pentagon
    ConvexPolyObject* testPent = new ConvexPolyObject(glm::vec3(0, -2.5, -12.5), glm::rotate(-90.0f, glm::vec3(1, 0, 0)), 5.0f, 5, true);
    testPent->ObjectColour = glm::vec4(1, 1, 0, 1);
    m_staticObjects.push_back(testPent);

    // Arrowhead
    std::vector<glm::vec3> arrowheadVertices;
    arrowheadVertices.push_back(glm::vec3(0, -1, 0));
    arrowheadVertices.push_back(glm::vec3(1, 1, 0));
    arrowheadVertices.push_back(glm::vec3(0, 1.5, 0));
    arrowheadVertices.push_back(glm::vec3(-1, 1, 0));
    ConvexPolyObject* arrowhead = new ConvexPolyObject(glm::vec3(-40, -10, -10), glm::rotate(-90.0f, glm::vec3(0, 1, 0)), 5.0f, arrowheadVertices, true);
    arrowhead->ObjectColour = glm::vec4(0, 1, 0, 1);
    m_staticObjects.push_back(arrowhead);

    // STATIC TREE
    initStaticTree();

    // DYNAMIC OBJECTS
    // Celestial Bodies
    sunSphere = new SphereObject(glm::vec3(), glm::mat4(), 500.0f, false);
    sunSphere->ObjectColour = glm::vec4(1, 1, 0, 1);
    sunSphere->CastShadow = false;
    sunSphere->Fullbright = true;
    m_dynamicObjects.push_back(sunSphere);

    moonSphere = new SphereObject(glm::vec3(), glm::mat4(), 500.0f, false);
    moonSphere->ObjectColour = glm::vec4(1, 1, 1, 1);
    moonSphere->CastShadow = false;
    moonSphere->Fullbright = true;
    m_dynamicObjects.push_back(moonSphere);

    // DYNAMIC TREE
    initDynamicTree();
}

TestScene::~TestScene()
{
    delete m_staticTree;
    delete m_dynamicTree;
}

void TestScene::Update(const float deltaTime)
{
    // Move celestial bodies
    float sunRotation = SUN_ROTATION_PER_SEC * deltaTime;

    SunNormal = glm::normalize(glm::rotateZ(SunNormal, sunRotation));
    sunSphere->SetPosition(-SunNormal * 5000.0f);
    moonSphere->SetPosition(SunNormal * 5000.0f);

    // Update Dynamic Tree
    updateDynamicTree();
}
