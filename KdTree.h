#ifndef KDTREE_H
#define KDTREE_H

#include <vector>

#include "WorldObject.h"
#include "PlaneObject.h"

struct KdNode
{
    AABB Bounds;
    KdNode* Parent = NULL;
    KdNode* LeftChild = NULL;
    KdNode* RightChild = NULL;
    int Axis = 0;
    bool Root = false;
    bool Leaf = false;

    KdNode(const AABB& bounds, const KdNode* parent, const int axis)
    {
        Bounds = bounds;
        Parent = (KdNode*)parent;
        Axis = axis;
    }

    ~KdNode()
    {
        if(LeftChild != NULL)
        {
            delete LeftChild;
        }

        if(RightChild != NULL)
        {
            delete RightChild;
        }
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
    KdTree(const std::vector<WorldObject*>& worldObjects);
    ~KdTree();

    KdNode* GetRootNode() const { return m_rootNode; }

    void GenTree(const std::vector<WorldObject*>& worldObjects);
    KdNode* NearestNeighbour(const glm::vec3& position, const KdNode* rootNode);

    std::vector<WorldObject*> SplittingPlanes;

private:
    KdNode* genNode(const std::vector<WorldObject*>& worldObjects, const KdNode* parentNode, const int depth);
    KdNode* depthFirst(const glm::vec3& position, const KdNode* currentNode, const int depth);

    KdNode* m_rootNode;
};

#endif // KDTREE_H
