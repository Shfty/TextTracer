/*
    Scene - A container for objects and their kD Trees
    Base class has no implementation-
    refer to method comments for instructions
*/

#include "Scene.h"

Scene::Scene()
{
    // Setup objects and add to static/dynamic vectors

    // Call initStaticTree and initDynamicTree
}

Scene::~Scene()
{
    // Delete any allocated objects
}

void Scene::Update(const int elapsedTime)
{
    // Update dynamic objects

    // Call genDynamicTree
}

void Scene::initStaticTree()
{
    m_staticTree = new KdTree(m_staticObjects);
}

void Scene::initDynamicTree()
{
    m_dynamicTree = new KdTree(m_dynamicObjects);
}

void Scene::updateDynamicTree()
{
    m_dynamicTree->GenTree(m_dynamicObjects);
}
