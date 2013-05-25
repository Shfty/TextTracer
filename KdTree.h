#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "WorldObject.h"

struct KdNode
{
    WorldObject* Object;
    KdNode* Parent;
    KdNode* LeftChild;
    KdNode* RightChild;
    int Axis = 0;
    bool Root = false;
    bool Leaf = false;

    KdNode(WorldObject* object, KdNode* parent, int axis)
    {
        Object = object;
        Parent = parent;
        Axis = axis;
    }

    ~KdNode()
    {
        delete Object;
        delete LeftChild;
        delete RightChild;
    }
};

struct xAxisSortPredicate
{
    bool operator()( const WorldObject* lx, const WorldObject* rx ) const {
    	return lx->GetPosition().x < rx->GetPosition().x;
    }
};

struct yAxisSortPredicate
{
    bool operator()( const WorldObject* lx, const WorldObject* rx ) const {
    	return lx->GetPosition().y < rx->GetPosition().y;
    }
};

struct zAxisSortPredicate
{
    bool operator()( const WorldObject* lx, const WorldObject* rx ) const {
    	return lx->GetPosition().z < rx->GetPosition().z;
    }
};

class KdTree
{
public:
    KdTree() {}
    KdTree(std::vector<WorldObject*> worldObjects);
    ~KdTree();

    KdNode* GetRootNode() const { return m_rootNode; }

    void GenTree(std::vector<WorldObject*> worldObjects);
    KdNode* NearestNeighbour(glm::vec3 position, KdNode* rootNode);

private:
    KdNode* genNode(std::vector<WorldObject*>& worldObjects, KdNode* parentNode, int depth);
    KdNode* depthFirst(glm::vec3 position, KdNode* currentNode, int depth);

    KdNode* m_rootNode;
};

#endif // KDTREE_H
