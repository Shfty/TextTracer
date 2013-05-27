#include <algorithm>

#include "KdTree.h"

KdTree::KdTree(const std::vector<WorldObject*>& worldObjects)
{
    GenTree(worldObjects);
}

KdTree::~KdTree()
{
    delete m_rootNode;
}

void KdTree::GenTree(const std::vector<WorldObject*>& worldObjects)
{
    m_rootNode = genNode(worldObjects, NULL, 0);
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

KdNode* KdTree::genNode(const std::vector<WorldObject*>& worldObjects, const KdNode* parentNode, const int depth)
{
    if(worldObjects.size() == 0) return NULL;

    int axis = depth % 3;

    std::vector<WorldObject*> sortedObjects = worldObjects;

    switch(axis)
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

    float axisAverage = (sortedObjects[0]->GetPosition()[axis] + sortedObjects[0]->GetPosition()[axis]) / 2;
    std::vector<WorldObject*> leftObjects;
    std::vector<WorldObject*> rightObjects;

    glm::vec3 minBound = sortedObjects[0]->GetPosition();
    glm::vec3 maxBound = minBound;

    for(uint16_t i = 0; i < sortedObjects.size(); i++)
    {
        WorldObject* object = sortedObjects[i];
        glm::vec3 objectPosition = object->GetPosition();
        for(uint16_t o = 0; o < 3; o++)
        {
            if(objectPosition[o] < minBound[o])
            {
                minBound[o] = objectPosition[o];
            }

            if(objectPosition[o] > maxBound[o])
            {
                maxBound[o] = objectPosition[o];
            }
        }

        if(objectPosition[axis] < axisAverage)
        {
            leftObjects.push_back(worldObjects[i]);
        }
        else if(objectPosition[axis] > axisAverage)
        {
            rightObjects.push_back(worldObjects[i]);
        }
    }

    AABB box = AABB(minBound, maxBound);
    KdNode* node = new KdNode(box, parentNode, axis);

    if(parentNode == NULL)
    {
        node->Root = true;
    }

    if(leftObjects.size() + rightObjects.size() > 0)
    {
        node->LeftChild = genNode(leftObjects, node, depth + 1);
        node->RightChild = genNode(rightObjects, node, depth + 1);
    }
    else
    {
        node->Leaf = true;
    }
/*
    glm::vec3 nodePosition = node->Object->GetPosition();
    std::cout << (node->Leaf ? "Leaf " : "") << (node->Root ? "Root " : "") << "Node at depth " << depth << ": X: " << nodePosition.x << " Y: " << nodePosition.y << " Z: " << nodePosition.z << std::endl;
*/
    return node;
}
