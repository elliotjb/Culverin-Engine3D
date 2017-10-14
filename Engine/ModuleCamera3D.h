#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	update_status UpdateConfig(float dt);
	bool CleanUp();

	void ShowPerformance(int ms_index);

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void LookObject(const vec3 &Spot);
	void LookAndMoveToObject();
	bool isMouseOnWindow();
	void Move(const vec3 &Movement);
	void MoveAt(const vec3 &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();
	void CheckOut();

public:
	
	vec3 X, Y, Z, Position, Reference;

	mat4x4 vehicle_info;

	btVector3 temp;
	vec3 player_pos;
	vec3 camera_pos;
	vec3 vec_view;
	bool changecam = false;

	bool CanMoveCamera = false;

private:
	bool canOut = false;
	float moveWithScroll = 0;
	mat4x4 ViewMatrix, ViewMatrixInverse;
};