#pragma once
#include "Geometry/AABB.h"
#include <list>

#define QUADTREE_MAX_ITEMS 8
#define QUADTREE_MIN_SIZE 10.0f

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();

	bool isLeaf() const;

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	void CreateChilds();
	void RedistributeChilds();


public:
	AABB box;
	std::list<GameObject*> objects;
	QuadtreeNode* parent = nullptr;
	QuadtreeNode* childs[4];

};


class Quadtree
{
public:
	Quadtree();
	virtual ~Quadtree();

	void Boundaries(AABB limits);
	void Clear();

	void Insert(GameObject* obj);
	void Remove(GameObject* obj);

	//void Intersect(std::vector<GameObject*>& vec, PRIMITIVE);

public:
	QuadtreeNode* root_node = nullptr;
};