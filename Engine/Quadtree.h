#pragma once
#include "Geometry/AABB.h"
#include <list>

#define QUADTREE_MAX_ITEMS 2
#define QUADTREE_MIN_SIZE 10.0f

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box, QuadtreeNode* parent);
	virtual ~QuadtreeNode();

	bool isLeaf() const;

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void Clear();

	void DebugDraw();

	void CreateChilds();
	void RedistributeChilds();


public:

	AABB box;	// container to check colisions
	std::list<GameObject*> objects;	// Objects inside this node
	QuadtreeNode* parent = nullptr; // From which node comes this node
	QuadtreeNode* childs[4]; // 4 child nodes (it's a Quadtree)

};


class Quadtree
{
public:
	Quadtree();
	virtual ~Quadtree();

	void Boundaries(AABB limits);
	void Clear();
	void Bake(const std::vector<GameObject*>& objects);
	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void DebugDraw();

	template<typename TYPE>
	void CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const;

public:
	QuadtreeNode* root_node = nullptr;
};

template<typename TYPE>
inline void Quadtree::CollectIntersections(std::vector<GameObject*>& objects, const TYPE& primitive) const
{
	if (root_node != nullptr)
	{
		root_node->CollectIntersections(objects, primitive);
	}
}