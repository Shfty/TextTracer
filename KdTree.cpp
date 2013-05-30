#include <algorithm>

#include "KdTree.h"
#include "ConvexPolyObject.h"

KdTree::KdTree(const AABB& bounds, const std::vector<WorldObject*>& worldObjects)
{
    GenTree(bounds, worldObjects);
}

KdTree::~KdTree()
{
    delete m_rootNode;
}

void KdTree::GenTree(const AABB& bounds, const std::vector<WorldObject*>& worldObjects)
{
    m_rootNode = genNode(bounds, worldObjects, NULL, 0);
}

/*
KdNode* KdTree::NearestNeighbour(const glm::vec3& position, const KdNode* rootNode)
{
    // Go depth-first to the bottom of the tree to get our initial estimate
    KdNode* bestEstimate = depthFirst(position, rootNode, 0);

    // Unwind the tree's recursion
    KdNode* currentNode = bestEstimate;
    while(currentNode != rootNode)
    {
        // If the distance to the current node is less than the best,
        // assign it to the best and continue
        glm::vec3 currentDistance = currentNode->Object->GetPosition() - position;
        glm::vec3 bestDistance = bestEstimate->Object->GetPosition() - position;

        if(glm::dot(currentDistance, currentDistance) < glm::dot(bestDistance, bestDistance))
        {
            bestEstimate = currentNode;
            //glm::vec3 bestDistance = bestEstimate->Object->GetPosition() - position;
        }

        // Check for closer points on the other side of the plane
        // TODO

        // Move one step up the tree
        currentNode = currentNode->Parent;
    }

    return bestEstimate;
}

KdNode* KdTree::depthFirst(const glm::vec3& position, const KdNode* currentNode, const int depth)
{
    if(currentNode->Leaf) return (KdNode*)currentNode;

    int axis = depth % 3;

    if(position[axis] >= currentNode->Object->GetPosition()[axis])
    {
        if(currentNode->LeftChild != NULL)
        {
            return depthFirst(position, currentNode->LeftChild, depth + 1);
        }
        else
        {
            return depthFirst(position, currentNode->RightChild, depth + 1);
        }
    }

    if(position[axis] < currentNode->Object->GetPosition()[axis])
    {
        if(currentNode->RightChild != NULL)
        {
            return depthFirst(position, currentNode->RightChild, depth + 1);
        }
        else
        {
            return depthFirst(position, currentNode->LeftChild, depth + 1);
        }
    }

    return NULL;
}
*/

KdNode* KdTree::genNode(const AABB& bounds, const std::vector<WorldObject*>& worldObjects, const KdNode* parentNode, const int depth)
{
    if(worldObjects.size() == 0) return NULL;

    // Sort the objects from near-far in the current splitAxis
    int splitAxis = depth % 3;
    std::vector<WorldObject*> sortedObjects = worldObjects;
    switch(splitAxis)
    {
    case 0:
        std::sort(sortedObjects.begin(), sortedObjects.end(), xAxisSortPredicate());
        break;
    case 1:
        std::sort(sortedObjects.begin(), sortedObjects.end(), yAxisSortPredicate());
        break;
    case 2:
        std::sort(sortedObjects.begin(), sortedObjects.end(), zAxisSortPredicate());
        break;
    default:
        break;
    }

    // Find the average of the objects positions in the current splitAxis
    float splitPosition = 0;
    for(uint16_t i = 0; i < sortedObjects.size(); i++)
    {
        splitPosition += sortedObjects[i]->GetPosition()[splitAxis];
    }
    splitPosition /= sortedObjects.size();

    // Sort the objects into left/right relative to the split position
    std::vector<WorldObject*> leftObjects;
    std::vector<WorldObject*> rightObjects;
    for(uint16_t i = 0; i < sortedObjects.size(); i++)
    {
        glm::vec3 objectPosition = sortedObjects[i]->GetPosition();

        if(objectPosition[splitAxis] < splitPosition)
        {
            leftObjects.push_back(worldObjects[i]);
        }
        else if(objectPosition[splitAxis] > splitPosition)
        {
            rightObjects.push_back(worldObjects[i]);
        }
    }

    // Generate node
    KdNode* node = new KdNode(splitAxis, splitPosition, parentNode);

    if(leftObjects.size() > 0)
    {
        AABB leftBounds;
        leftBounds.Min = bounds.Min;
        leftBounds.Max = bounds.Max;
        leftBounds.Max[splitAxis] = splitPosition;
        node->LeftChild = genNode(leftBounds, leftObjects, node, depth + 1);
    }

    if(rightObjects.size() > 0)
    {
        AABB rightBounds;
        rightBounds.Min = bounds.Min;
        rightBounds.Max = bounds.Max;
        rightBounds.Min[splitAxis] = splitPosition;
        node->RightChild = genNode(rightBounds, rightObjects, node, depth + 1);
    }

    if(leftObjects.size() + rightObjects.size() == 0)
    {
        node->Leaf = true;
    }

    node->Bounds = bounds;

    return node;
}
