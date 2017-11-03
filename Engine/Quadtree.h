#pragma once
#include "Geometry/AABB.h"
#include <list>

class GameObject;

class QuadtreeNode
{
public:
	QuadtreeNode(const AABB& box);
	virtual ~QuadtreeNode();



private:
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



private:
	QuadtreeNode* root = nullptr;
};