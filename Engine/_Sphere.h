#pragma once
#include "BaseGeometry.h"
#include "MathGeoLib.h"
#include <vector>

class _Sphere :public BaseGeometry
{
public:
	_Sphere(Prim_Type t, const float3 p, const float r, bool k, Color c, uint def, bool w);
	~_Sphere();

	void Init();
	void SetDefinition(uint def);
	void Draw();

public:
	Mesh sphere_mesh;
	float radius = 0;
	int definition = 0;
	Sphere* sphere = nullptr;

	std::vector<float3> vertex_array;
};