#include <algorithm>

#include "KdTree.h"

KdTree::KdTree(std::vector<WorldObject*> worldObjects)
{
    GenTree(worldObjects);
}

KdTree::~KdTree()
{
    delete m_rootNode;
}

void KdTree::GenTree(std::vector<WorldObject*> worldObjects)
{
    m_rootNode = genNode(worldObjects, NULL, 0);
}

KdNode* KdTree::NearestNeighbour(glm::vec3 position, KdNode* rootNode)
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
            glm::vec3 bestDistance = bestEstimate->Object->GetPosition() - position;
        }

        // Check for closer points on the other side of the plane
        // TODO

        // Move one step up the tree
        currentNode = currentNode->Parent;
    }

    return bestEstimate;
}

KdNode* KdTree::depthFirst(glm::vec3 position, KdNode* currentNode, int depth)
{
    if(currentNode->Leaf) return currentNode;

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
}


KdNode* KdTree::genNode(std::vector<WorldObject*>& worldObjects, KdNode* parentNode, int depth)
{
    if(worldObjects.size() == 0) return NULL;

    int axis = depth % 3;

    switch(axis)
    {
    case 0:
        std::sort(worldObjects.begin(), worldObjects.end(), xAxisSortPredicate());
        break;
    case 1:
        std::sort(worldObjects.begin(), worldObjects.end(), yAxisSortPredicate());
        break;
    case 2:
        std::sort(worldObjects.begin(), worldObjects.end(), zAxisSortPredicate());
        break;
    default:
        break;
    }

    int medianIdx = worldObjects.size() / 2;
    std::vector<WorldObject*> leftObjects;
    std::vector<WorldObject*> rightObjects;

    for(int i = 0; i < worldObjects.size(); i++)
    {
        if(i < medianIdx)
        {
            leftObjects.push_back(worldObjects[i]);
        }
        else if(i > medianIdx)
        {
            rightObjects.push_back(worldObjects[i]);
        }
    }

    KdNode* node = new KdNode(worldObjects[medianIdx], parentNode, axis);
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
