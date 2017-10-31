#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "_Model.h"
#include "BaseGeometry.h"

class GameObject;
class CompCamera;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	update_status UpdateNew(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	// REMOVE THIS ------------------------------
	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void LookObject(const vec3 &Spot);
	void LookAndMoveToObject();
	
	void Move(const vec3 &Movement);
	void MoveAt(const vec3 &Movement);
	void Rotate(float dx, float dy);
	// ----------------------------------------

	void MoveWithKeyboard(float dt);
	void MoveWithMouse(float dx, float dy, float dt);

	void Orbit(float dx, float dy);
	void LookAt(const float3& spot);
	void LookAround(float dx, float dy);
	void Zoom(int zoom);
	void CenterToObject();

	
	bool isMouseOnWindow();

	float* GetViewMatrix();

	//void SetRay(float mouseX, float mouseY, float W, float H);

private:
	void CalculateViewMatrix();
	void CheckOut();
	void ReajustLook();

public:
	vec3 X, Y, Z, Position, Reference;
	bool changecam = false;
	bool CanMoveCamera = false;

private:
	CompCamera* cam = nullptr;

	GameObject* focus = nullptr;
	float3 point_to_look = { 0, 0, 0 };
	BaseGeometry* geo = nullptr;

	LineSegment mouse_ray;

	// Camera Movement ------------
	float3 cam_move = { 0, 0, 0 };
	float move_speed = 1.0f;
	float rotate_speed = 1.0f;
	float scroll_speed = 1.0f;

	bool canOut = false;
	bool needReajust = false;

	mat4x4 ViewMatrix, ViewMatrixInverse;
};