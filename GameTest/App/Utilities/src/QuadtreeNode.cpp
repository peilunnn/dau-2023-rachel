#include "stdafx.h"
#include "Utilities/include/QuadtreeNode.h"

QuadtreeNode::QuadtreeNode(int level, float x, float y, float width, float height)
    : level(level), x(x), y(y), width(width), height(height) 
{
    for (int i = 0; i < 4; i++)
        nodes[i] = nullptr;
}

QuadtreeNode::~QuadtreeNode() {
    Clear();
}

void QuadtreeNode::Clear() {
    elements.clear();
    for (int i = 0; i < 4; i++) 
    {
        if (nodes[i] != nullptr) 
        {
            nodes[i]->Clear();
            nodes[i] = nullptr;
        }
    }
}

void QuadtreeNode::Split() {
    float widthHalved = width / 2.0f;
    float heightHalved = height / 2.0f;
    float xMidpoint = x + widthHalved;
    float yMidpoint = y + heightHalved;

    nodes[0] = make_unique<QuadtreeNode>(level + 1, x, y, widthHalved, heightHalved); // Top Left
    nodes[1] = make_unique<QuadtreeNode>(level + 1, xMidpoint, y, widthHalved, heightHalved); // Top Right
    nodes[2] = make_unique<QuadtreeNode>(level + 1, x, yMidpoint, widthHalved, heightHalved); // Bottom Left
    nodes[3] = make_unique<QuadtreeNode>(level + 1, xMidpoint, yMidpoint, widthHalved, heightHalved); // Bottom Right
}

int QuadtreeNode::GetIndex(Transform* transform) 
{
    /*
    +-----------+------------+
    |  Top Left | Top Right  |
    |  (index 0)| (index 1)  |
    +-----------+------------+
    |Bottom Left|Bottom Right|
    |  (index 2)|  (index 3) |
    +-----------+-----------+ 
    */

    int index = -1;
    float verticalMidpoint = x + (width / 2.0f);
    float horizontalMidpoint = y + (height / 2.0f);

    bool isInTopQuadrant = (transform->GetPosition().y < horizontalMidpoint);
    bool isInBottomQuadrant = (transform->GetPosition().y > horizontalMidpoint);

    // If in left half
    if (transform->GetPosition().x < verticalMidpoint) {
        if (isInTopQuadrant)
            index = 0; // Top Left
        else if (isInBottomQuadrant)
            index = 2; // Bottom Left
    }

    // If in right half
    else if (transform->GetPosition().x > verticalMidpoint) 
    {
        if (isInTopQuadrant)
            index = 1; // Top Right
        else if (isInBottomQuadrant)
            index = 3; // Bottom Right
    }

    return index;
}

void QuadtreeNode::Insert(Transform* transform, EntityId entityId)
{
    QuadtreeElement element(transform, entityId);

    // If child nodes exist, determine which child node the transform should belong to
    if (nodes[0] != nullptr) 
    {
        int index = GetIndex(transform);

        if (index != -1) {
            nodes[index]->Insert(transform, entityId);
            return;
        }
    }

    elements.push_back(element);

    // After we add object to node, check if node exceeded capacity. If it did, we need to split
    if (elements.size() > maxObjects && level < maxDepth)
    {
        if (nodes[0] == nullptr)
            Split();

        // After splitting, redistribute elements to child nodes
        int i = 0;
        while (i < elements.size()) 
        {
            int index = GetIndex(elements[i].transform);

            // If we get a valid index, move object into child node
            if (index != -1) 
            {
                nodes[index]->Insert(elements[i].transform, elements[i].entityId);
                elements.erase(elements.begin() + i);
            }
            
            // Otherwise object remains in current node
            else 
            {
                i++;
            }
        }
    }
}

vector<QuadtreeElement> QuadtreeNode::Retrieve(Transform* transform) 
{
    vector<QuadtreeElement> returnElements;
    int index = GetIndex(transform);
    
    if (index != -1 && nodes[0] != nullptr)
        returnElements = nodes[index]->Retrieve(transform);

    returnElements.insert(returnElements.end(), elements.begin(), elements.end());

    return returnElements;
}
