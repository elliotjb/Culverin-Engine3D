#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "_Model.h"
#include "BaseGeometry.h"
#include <map>

class GameObject;
class CompCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	void MoveWithKeyboard(float dt);
	void MoveWithMouse(int dx, int dy, float dt);

	void Orbit(float dx, float dy);
	void LookAt(const float3& spot);
	void LookAround(float dx, float dy);
	void Zoom(float zoom);

	void MousePick(float x, float y, float w, float h);
	float3 IntersectionPoint(const AABB* box);

	void SetFocus(const GameObject* selected);
	void CenterToObject();

	bool isMouseOnWindow();


private:
	void CheckOut();

public:
	bool changecam = false;
	bool CanMoveCamera = false;
	LineSegment ray;

private:
	CompCamera* cam = nullptr;
	const GameObject* focus = nullptr;
	float3 point_to_look = { 0, 0, 0 };

	std::map<float, GameObject*> possible_intersections;

	// Camera Movement ------------
	float3 cam_move = { 0, 0, 0 };
	float move_speed = 1.0f;
	float rotate_speed = 1.0f;
	float scroll_speed = 1.0f;

	bool canOut = false;
};