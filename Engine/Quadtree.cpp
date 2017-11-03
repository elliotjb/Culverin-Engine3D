#include "Quadtree.h"

// QUADTREE NODE -------------------------

QuadtreeNode::QuadtreeNode(const AABB& box): box(box)
{
	parent = nullptr;

	for (uint i = 0; i < 4; i++)
	{
		childs[i] = nullptr;
	}
}

QuadtreeNode::~QuadtreeNode()
{
}

// ---------------------------------------


// QUADTREE ------------------------------

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
}

// --------------------------------------
