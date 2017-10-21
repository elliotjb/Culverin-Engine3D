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

class CompTransform :public Component
{
public:
	CompTransform(Comp_Type t);
	~CompTransform();

	void Init(float3 p, float3 r, float3 s);

	void ShowInspectorInfo();

	void SetPos(float3 pos);
	void SetRot(float3 rot);
	void SetScale(float3 scale);

	float3 GetPos() const;
	float3 GetRot() const;
	float3 GetScale() const;

private:
	Axis axis;
	float3 position = { 0, 0, 0 };
	float3 rotation = { 0, 0, 0 };
	float3 scale = { 0, 0, 0 };
};