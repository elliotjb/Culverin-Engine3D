#pragma once
#include "BaseGeometry.h"
#include "MathGeoLib.h"
#include <vector>

class _Cylinder :public BaseGeometry
{
public:

	_Cylinder(Prim_Type t, const float3 p, bool k, Color c, float lenght, float radius, bool w);
	~_Cylinder();

	void Create();

	void MakeCylinder(float radius, float length, unsigned int numSteps);

	//bool Start();
	//update_status Update(float dt);
	//bool CleanUp();

public:

	std::vector<float3> vertex;
	float3 index;
	float lenght;
	float radius;
};
