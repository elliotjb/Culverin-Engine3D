#pragma once
#include "BaseObject.h"
#include "MathGeoLib.h"
#include <vector>

class _Sphere :public BaseObject
{
public:
	_Sphere(Prim_Type t, const float3 p, const float r, bool k, Color c, uint def, bool w);
	~_Sphere();

	void SetDefinition(uint def);

public:
	float radius = 0;
	int definition = 0;
	Sphere* sphere = nullptr;

	std::vector<float3> vertex_array;
};