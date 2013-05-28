#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"
#include "SphereObject.h"

class TestScene : public Scene
{
public:
    TestScene();
    ~TestScene();

   void Update(const float deltaTime);

private:
    std::vector<WorldObject*> m_debugObjects;
};

#endif // TESTSCENE_H
