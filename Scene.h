#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "WorldObject.h"
#include "KdTree.h"

class Scene
{
public:
    Scene();
    virtual ~Scene() = 0;

    const std::vector<WorldObject*>* GetStaticObjects() const { return &m_staticObjects; };
    const std::vector<WorldObject*>* GetDynamicObjects() const { return &m_dynamicObjects; };
    KdTree* GetStaticTree() const { return m_staticTree; };
    KdTree* GetDynamicTree() const { return m_dynamicTree; };

    virtual void Update(const float deltaTime);

    glm::vec3 SunNormal = glm::vec3(0, 1, 0);

protected:
    void initStaticTree();
    void initDynamicTree();
    void updateDynamicTree();

    KdTree* m_staticTree;
    KdTree* m_dynamicTree;
    std::vector<WorldObject*> m_staticObjects;
    std::vector<WorldObject*> m_dynamicObjects;
};

#endif // SCENE_H
