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
    float SplitPosition = 0;
    int SplitAxis = 0;
    bool Leaf = false;

    KdNode(const int splitAxis, const float splitPos, const KdNode* parent)
    {
        SplitAxis = splitAxis;
        SplitPosition = splitPos;
        Parent = (KdNode*)parent;
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
    KdTree(const AABB& bounds, const std::vector<WorldObject*>& worldObjects);
    ~KdTree();

    KdNode* GetRootNode() const { return m_rootNode; }

    void GenTree(const AABB& bounds, const std::vector<WorldObject*>& worldObjects);
    //KdNode* NearestNeighbour(const glm::vec3& position, const KdNode* rootNode);

private:
    KdNode* genNode(const AABB& bounds, const std::vector<WorldObject*>& worldObjects, const KdNode* parentNode, const int depth);
    KdNode* depthFirst(const glm::vec3& position, const KdNode* currentNode, const int depth);

    KdNode* m_rootNode;
};

#endif // KDTREE_H
