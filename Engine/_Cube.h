#pragma once
#include "BaseGeometry.h"
#include "MathGeoLib.h"
#include <vector>

class _Cube :public BaseGeometry
{
public:
	_Cube(Prim_Type t, const float3 p, const float3 size, bool k, Color c, bool w);
	~_Cube();

	void Init();
	void Draw();

public:
	Mesh cube_mesh;
	float3 size = { 0, 0, 0 };
	OBB* boxed = nullptr;
	AABB* box = nullptr;
	std::vector<float3> vertex_array;
};