#pragma once
#include "Components/include/Transform.h"
#include "Utilities/include/EntityId.h"
#include <vector>
#include <memory>
using std::unique_ptr;
using std::vector;

struct QuadtreeElement 
{
    Transform* transform;
    EntityId entityId;

    QuadtreeElement(Transform* t, EntityId id) : transform(t), entityId(id) {}
};

class QuadtreeNode 
{
public:
    QuadtreeNode(int level, float x, float y, float width, float height);
    ~QuadtreeNode();

    void Clear();
    void Insert(Transform* transform, EntityId id);
    vector<QuadtreeElement> Retrieve(Transform* transform);

private:
    int maxObjects = 10;
    int maxDepth = 5;

    int level;
    vector<QuadtreeElement> elements;
    float x, y;
    float width, height;
    unique_ptr<QuadtreeNode> nodes[4];

    void Split();
    int GetIndex(Transform* transform);
};
