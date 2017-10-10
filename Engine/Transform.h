#pragma once
#include "Component.h"
#include "MathGeoLib.h"
#include <vector>

struct Axis
{
	float3 x = { 1, 0, 0 };
	float3 y = { 0, 1, 0 };
	float3 z = { 0, 0, 1 };
};

class Transform :public Component
{
public:
	Transform(Comp_Type t);
	~Transform();

	void SetPos(float3 pos);
	void SetRot(float3 rot);
	void SetSize(float3 size);

private:
	Axis axis;
	float3 position = { 0, 0, 0 };
	float3 rotation = { 0, 0, 0 };
	float3 size = { 0, 0, 0 };
};