#ifndef TESTSCENE_H
#define TESTSCENE_H

#include "Scene.h"
#include "SphereObject.h"

class TestScene : public Scene
{
public:
    TestScene();
    ~TestScene();

   void Update(int elapsedTime);
};

#endif // TESTSCENE_H
