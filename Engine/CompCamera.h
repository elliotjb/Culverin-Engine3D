#pragma once
#include "Component.h"
#include "Geometry/Frustum.h"

class CompCamera : public Component
{
public:
	CompCamera(Comp_Type t);
	~CompCamera();

	void Init(float3 pos);
	void preUpdate();
	void Update();
	void DebugDraw();

	void ShowInspectorInfo();


	void SetPos(float3 pos);
	void SetNear(float near_p);
	void SetFar(float far_p);
	void SetFov(float vertical);

private:
	bool culling = false;

	float width = 0.0f;
	float height = 0.0f;
	float aspect_ratio = 0.0f;

	Frustum cam_frustum;
	float near_plane = 0.0f;
	float far_plane = 0.0f;
	float vertical_fov = 0.0f;
};