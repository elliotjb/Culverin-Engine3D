#include "_Cube.h"
#include "BaseObject.h"
#include "MathGeoLib.h"
#include "GL3W\include\glew.h""
#include <vector>

_Cube::_Cube(Prim_Type t, const float3 p, const float3 s, bool k, Color c) :BaseObject(t, p, k, c)
{
	size = s;

	boxed = new OBB();
	boxed->pos = pos;
	boxed->r = size;
	boxed->axis[0] = float3(1, 0, 0);
	boxed->axis[1] = float3(0, 1, 0);
	boxed->axis[2] = float3(0, 0, 1);

	AABB boxer(*boxed);
	box = &boxer;

	vertex_array.reserve(36);
	box->Triangulate(1, 1, 1, vertex_array.data(), NULL, NULL, false);
}

_Cube::~_Cube()
{
}

